#include "unity.h"

#include "NMEAParser.c"
#include "NMEAParser.h"

#include <string.h>
#include "mock_freeRTOS.h"

#include <stdio.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_NMEAParser_will_take_input_sentence(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();

    xTaskGetTickCountFromISR_ExpectAndReturn(0);

    char *NMEA_to_decode = "$GPRMC,165401.997831,A,3725.0690,N,12203.3824,W,0.000,082.1,070717,,,A*48\r\n";

    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        } else {
            TEST_ASSERT_FALSE(new_data);
        }
        
    }
}


void test_NMEAParser_will_decode_GPRMC(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
    xTaskGetTickCountFromISR_ExpectAndReturn(0);
    xTaskGetTickCount_ExpectAndReturn(100);

    char *NMEA_to_decode = "$GPRMC,165401.997831,A,3725.0690,N,12203.3824,W,0.000,082.1,070717,,,A*48\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        }
    }
    GPRMC_s GPRMC_test;
    NMEAParser_get_GPRMC(&GPRMC_test);
    TEST_ASSERT_EQUAL_UINT32(16540199, GPRMC_test._time);
    TEST_ASSERT_EQUAL_INT32(37, GPRMC_test._latitude_upper);
    TEST_ASSERT_EQUAL_INT32(4178166, GPRMC_test._latitude_lower);
    TEST_ASSERT_EQUAL_INT32(-122, GPRMC_test._longitude_upper);
    TEST_ASSERT_EQUAL_INT32(563733, GPRMC_test._longitude_lower);
    TEST_ASSERT_EQUAL_INT32(0, GPRMC_test._speed);
    TEST_ASSERT_EQUAL_INT32(8210, GPRMC_test._course);
    TEST_ASSERT_EQUAL_INT32(70717, GPRMC_test._date);



    // This code will test some code paths that aren't tested in the other
    // test. such as having a speed value and alternate N/S and E/W
    xTaskGetTickCountFromISR_ExpectAndReturn(0);
    xTaskGetTickCount_ExpectAndReturn(100);
    NMEA_to_decode = "$GPRMC,165401.997831,A,3725.0690,S,12203.3824,E,1156.433,082.1,070717,,,A*70\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        }
    }
    NMEAParser_get_GPRMC(&GPRMC_test);

    TEST_ASSERT_EQUAL_UINT32(16540199, GPRMC_test._time);
    TEST_ASSERT_EQUAL_INT32(-37, GPRMC_test._latitude_upper);
    TEST_ASSERT_EQUAL_INT32(4178166, GPRMC_test._latitude_lower);
    TEST_ASSERT_EQUAL_INT32(122, GPRMC_test._longitude_upper);
    TEST_ASSERT_EQUAL_INT32(563733, GPRMC_test._longitude_lower);
    TEST_ASSERT_EQUAL_INT32(115643, GPRMC_test._speed);
    TEST_ASSERT_EQUAL_INT32(8210, GPRMC_test._course);
    TEST_ASSERT_EQUAL_INT32(70717, GPRMC_test._date);
    TEST_ASSERT_EQUAL_INT32(0, GPRMC_test._logtime);
}
void test_NMEAParser_will_decode_GPGGA(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
    xTaskGetTickCountFromISR_ExpectAndReturn(0);
    xTaskGetTickCount_ExpectAndReturn(100);

    char *NMEA_to_decode = "$GPGGA,165155.997704,3725.0673,N,12203.3815,W,1,08,1.2,26.8,M,-32.0,M,,0000*6E\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        }
    }
    GPGGA_s GPGGA_test;
    NMEAParser_get_GPGGA(&GPGGA_test);
    TEST_ASSERT_EQUAL_UINT32(16515599, GPGGA_test._time);
    TEST_ASSERT_EQUAL_INT32(37, GPGGA_test._latitude_upper);
    TEST_ASSERT_EQUAL_INT32(4177883, GPGGA_test._latitude_lower);
    TEST_ASSERT_EQUAL_INT32(-122, GPGGA_test._longitude_upper);
    TEST_ASSERT_EQUAL_INT32(563583, GPGGA_test._longitude_lower);
    TEST_ASSERT_EQUAL_INT32(1, GPGGA_test._fixquality);
    TEST_ASSERT_EQUAL_INT32(8, GPGGA_test._numsats);
    TEST_ASSERT_EQUAL_INT32(120, GPGGA_test._hdop);
    TEST_ASSERT_EQUAL_INT32(2680, GPGGA_test._altitude);
    TEST_ASSERT_EQUAL_INT32(0, GPGGA_test._logtime);
}

void test_NMEAParser_will_decode_GPGSA(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
    xTaskGetTickCountFromISR_ExpectAndReturn(0);
    xTaskGetTickCount_ExpectAndReturn(100);

    char *NMEA_to_decode = "$GPGSA,A,3,01,13,28,07,11,17,15,30,,,,,2.0,1.2,1.6*3B\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        }
    }
    GPGSA_s GPGSA_test;
    NMEAParser_get_GPGSA(&GPGSA_test);
    TEST_ASSERT_EQUAL_INT32(120, GPGSA_test._hdop);
    TEST_ASSERT_EQUAL_INT32(200, GPGSA_test._pdop);
    TEST_ASSERT_EQUAL_INT32(160, GPGSA_test._vdop);
    TEST_ASSERT_EQUAL_INT32(3, GPGSA_test._fixtype);
    TEST_ASSERT_EQUAL_INT32(0, GPGSA_test._logtime);

}

void test_NMEAParser_will_decode_GPGSV(void) {
    TEST_IGNORE_MESSAGE("need to learn how GPGSV works");

    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
    xTaskGetTickCountFromISR_ExpectAndReturn(0);
    xTaskGetTickCount_ExpectAndReturn(100);

    char *NMEA_to_decode = "$GPGSV,3,1,10,17,69,177,30,28,59,031,44,30,54,112,42,13,52,271,41*77\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_TRUE(new_data);
        }
    }
    GPGSV_s GPGSV_test;
    NMEAParser_get_GPGSV(&GPGSV_test);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._numsats_visible);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._gsv_sentence);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._gsv_sentences);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._snr_count);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._snr_total);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._new_snr_total); 
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._snr_avg);
    TEST_ASSERT_EQUAL_INT32(0, GPGSV_test._logtime);

}

void test_NMEAParser_returns_false_unknown_sentence(void) {
    // in this function we use a GPZDA example sentence. it is assumed the code will not
    // be able to decode it. If the functionality for this sentence is added this test will fail


    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
    xTaskGetTickCountFromISR_ExpectAndReturn(0);

    char *NMEA_to_decode = "$GPZDA,165154.997704,07,07,2017,00,00*64\r\n";
    for (int i = 0; i<strlen(NMEA_to_decode); i++) {
        bool new_data = false;

        // NMEAParser expects data to be given one char at a time
        // until \n is reached
        new_data = NMEAParser_encode(NMEA_to_decode[i]);
        if (NMEA_to_decode[i] == '\n'){
            TEST_ASSERT_FALSE(new_data);
        }
    }
}