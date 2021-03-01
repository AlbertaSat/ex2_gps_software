#include "sci.h"
#include "skyTraq_binary.h"
#include "NMEAParser.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include <string.h>
#include "os_queue.h"
#include "os_task.h"
#include "skytraq_gps_driver.h"

#define SKYTRAQ_UART sciREG
#define BUFSIZE 100

bool GGA_ENABLED = false;
bool GSA_ENABLED = false;
bool GSV_ENABLED = false;
bool RMC_ENABLED = false;

enum current_sentence {
    none,
    binary,
    nmea
} line_type;

#define ITEM_SIZE BUFSIZE
#define QUEUE_LENGTH 2
uint8_t inQueueStorage[ QUEUE_LENGTH * ITEM_SIZE ];
static StaticQueue_t xStaticQueue;
QueueHandle_t inQueue = NULL;

char binary_message_buffer[BUFSIZE];
int bin_buff_loc;

uint8_t byte;

bool sci_busy;

int current_line_type = none;

#define header_size 4
#define footer_size 3

// IMPORTANT: this function will delay for 30s due to cold start. This function is expected to run
// on a cold boot of the OBC. Call TODO: IMPLEMENT NEEDED FUNCTION TO CALL instead to warm start the skytraq
bool gps_skytraq_driver_init() {
    memset(binary_message_buffer, 0, BUFSIZE);
    bin_buff_loc = 0;
    //TODO: make this use xQueueCreateStatic
    sci_busy = false;
    inQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
    if (inQueue == NULL) {
        return false;
    }
    ErrorCode restart = skytraq_restart_receiver(COLD_START, 2020, 2, 10, 11, 40, 0, 0, 0,2000);
    if (restart != SUCCESS) {
        return false;
    }
    vTaskDelay(30000/portTICK_PERIOD_MS);
    ErrorCode powerMode = skytraq_configure_power_mode(POWERSAVE, UPDATE_TO_FLASH);
    if (powerMode != SUCCESS) {
        return false;
    }

    ErrorCode gps_enable_all = gps_configure_message_types(true,true,true,true,1);
    if (gps_enable_all != SUCCESS) {
        return false;
    }

    //TODO: maybe implement a way to request CRC and compare it with a stored CRC?
    return true;
}
ErrorCode gps_configure_message_types(bool GGA, bool GSA, bool GSV, bool RMC, uint8_t interval) {
    uint8_t GGA_int = GGA ? interval : 0;
    uint8_t GSA_int = GSA ? interval : 0;
    uint8_t GSV_int = GSV ? interval : 0;
    uint8_t RMC_int = RMC ? interval : 0;
    GGA_ENABLED = GGA;
    GSA_ENABLED = GSA;
    GSV_ENABLED = GSV;
    RMC_ENABLED = RMC;
    return skytraq_configure_nmea_output_rate(GGA_int, GSA_int, GSV_int, 0, RMC_int, 0,0, UPDATE_TO_FLASH);
}

ErrorCode gps_disable_NMEA_output() {
    return gps_configure_message_types(false,false,false,false, 0);
}

void extract_time(uint32_t _time, TickType_t correction, uint8_t *utc_hour, uint8_t *utc_minute, uint8_t *utc_second, uint16_t *utc_ms) {
    // _time is stored such that: hhmmssss

    int ms_since_logged = (xTaskGetTickCount()* portTICK_PERIOD_MS) - correction*portTICK_PERIOD_MS;
    *utc_hour = _time/1000000;
    *utc_minute = _time / 10000 % 100;
    *utc_second = _time / 100 % 100;
    *utc_ms = (_time % 100) * 10;

    // Apply time correction:
    *utc_ms += ms_since_logged;
    if (*utc_ms / 1000) {
        *utc_second += *utc_ms/1000;
        *utc_ms %= 1000;
    }
    if (*utc_second / 60) {
        *utc_minute += *utc_second/60;
        *utc_second %= 60;
    }
    if (*utc_minute / 60) {
        *utc_hour += *utc_minute/60;
        *utc_minute %= 60;
    }
    if (*utc_hour / 24) {
        *utc_hour %= 24;
    }
    return;
}

bool gps_get_utc_time(uint8_t *utc_hour, uint8_t *utc_minute, uint8_t *utc_second, uint16_t *utc_ms) {

    // this will take GPRMC time if it is available, otherwise GPGGA
    bool GGA = GGA_ENABLED;
    bool RMC = RMC_ENABLED;
    GPGGA_s GGA_s;
    GPRMC_s RMC_s;

    if (RMC) {
        bool RMC_valid = NMEAParser_get_GPRMC(&RMC_s);
        if (RMC_valid) {
            extract_time(RMC_s._time, RMC_s._logtime, utc_hour, utc_minute, utc_second, utc_ms);
            return true;
        }
    }

    if (GGA) {
        bool GGA_valid = NMEAParser_get_GPGGA(&GGA_s);
        if (GGA_valid) {
            extract_time(GGA_s._time, GGA_s._logtime, utc_hour, utc_minute, utc_second, utc_ms);
            return true;
        }
    }
    return false;
}

// Sending a message will block until a reply is received
// Task must not be waiting on a notification at index 0
ErrorCode skytraq_send_message(uint8_t *paylod, uint16_t size) {
    if (sci_busy) {
        return RESOURCE_BUSY;
    }
    sci_busy = true;
    int total_size = size+header_size+footer_size;
    uint8_t *message = pvPortMalloc(total_size);
    memset(message, 0, total_size);
    message[0] = 0xA0;
    message[1] = 0xA1;
    *(uint16_t *) &(message[2]) = size;
    memcpy(&(message[4]), paylod, size);
    message[total_size-3] = calc_checksum(message, size);
    message[total_size-1] = 0x0A;
    message[total_size-2] = 0x0D;


    sciSend(SKYTRAQ_UART, total_size, message);
    vPortFree(message);

    uint8_t sentence[BUFSIZE];

    // TODO: set a reasonable delay
    BaseType_t success = xQueueReceive(inQueue, sentence, portMAX_DELAY);

    if (success  == pdFALSE) {
        sci_busy = false;
        return UNKNOWN_ERROR;
    }

    bool cs_success = skytraq_verify_checksum(sentence);

    if (cs_success) {
        switch(sentence[4]){
        case 0x83: sci_busy = false; return SUCCESS;
        case 0x84: sci_busy = false; return MESSAGE_INVALID;
        }
    } else {
        sci_busy = false;
        return INVALID_CHECKSUM_RECEIVE;
    }
    // should never reach here... I pray there is a merciful God
    sci_busy = false;
    return UNKNOWN_ERROR;
}

ErrorCode skytraq_send_message_with_reply(uint8_t *payload, uint16_t size, uint8_t *reply) {
    ErrorCode worked = skytraq_send_message(payload, size);

    if (worked != SUCCESS) {
        return worked;
    }
    // WARNING: possible race condition where another task could get control
    // of the sci port in between send message returning and this function setting
    // value to busy.
    sci_busy = true;

    uint8_t sentence[BUFSIZE];

    // TODO: set a reasonable delay
    BaseType_t success = xQueueReceive(inQueue, sentence, portMAX_DELAY);

    if (success  == pdFALSE) {
        sci_busy = false;
        return UNKNOWN_ERROR;
    }
    bool cs_success = skytraq_verify_checksum(sentence);
    if (cs_success) {
        strcpy((char *)reply, (char *)sentence);
        sci_busy = false;
        return SUCCESS;
    } else {
        sci_busy = false;
        return INVALID_CHECKSUM_RECEIVE;
    }
    sci_busy = false;
}

// TODO: should I really keep this?
static inline increment_buffer(int *buf) {
    *buf += 1;
}

// Interrupt Handler
void get_byte() {
    uint8_t in = byte;

    switch(current_line_type) {

    case none:
        switch(in){
        case '$': current_line_type = nmea; NMEAParser_encode(in); break; // this means NMEA decoding happens in the isr. Have to determine if this is a problem
        case 0xA0: current_line_type = binary; binary_message_buffer[bin_buff_loc] = in; increment_buffer(&bin_buff_loc); break;
        }; break;

    case nmea: NMEAParser_encode(in); break;

    case binary: binary_message_buffer[bin_buff_loc] = in; increment_buffer(&bin_buff_loc); break;
    }

    if (in == '\n') {
        if (current_line_type == binary) {
            current_line_type = none;
            increment_buffer(&bin_buff_loc);
            binary_message_buffer[bin_buff_loc] = '\0';
            increment_buffer(&bin_buff_loc);
            xQueueSendFromISR(inQueue, binary_message_buffer, NULL);
            bin_buff_loc = 0;
            memset(binary_message_buffer, 0, BUFSIZE);
        } else {
            current_line_type = none;
        }
    }

}

void sciNotification(sciBASE_t *sci, unsigned flags) {
    switch(flags) {
    case SCI_RX_INT: get_byte(); sciReceive(sci, 1, &byte); break;

    case SCI_TX_INT: break;
    }
}


