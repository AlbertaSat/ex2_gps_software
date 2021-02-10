#include "sci.h"
#include "skyTraq_binary.h"
#include "NMEAParser.h"
#include <stdbool.h>

#define UART sciREG

enum current_sentence {
    none,
    binary,
    nmea
} line_type;

#define BUFSIZE 200
char input_buffer[BUFSIZE];
int in_buff_loc;

char output_buffer[BUFSIZE];
int out_buff_loc;

int current_line_type = none;

void get_byte() {
    unsigned char in = input_buffer[in_buff_loc]

    switch(current_line_type) {

    case none:
        switch(in){
        case '$': current_line_type = nmea; NMEAParser_encode(in); break
        case 0xA0: current_line_type = binary; break
        }
    }
    case nmea: NMEAParser_encode(in); break;
    case binary: break;

    in_buff_loc++;
}

void sciNotification(sciBASE_t *sci, unsigned flags) {
    switch(flags) {
    case SCI_RX_INT: get_byte(); sciReceive(sci, 1, (unsigned char *) &(input_buffer[in_buff_loc])); break;

    case SCI_TX_INT:
    }
}

