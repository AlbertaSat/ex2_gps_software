#include "unity.h"

#include "NMEAParser.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}




void test_NMEAParser_will_take_input_sentence(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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

void test_NMEAParser_parse_degrees(void) {
    TEST_IGNORE_MESSAGE("need to implement degree parse function test");
}

void test_NMEAParser_parse_decimal(void) {
    TEST_IGNORE_MESSAGE("need to implement decimal parse function test");
}

void test_NMEAParser_hexToInt(void) {
    TEST_IGNORE_MESSAGE("need to implement hexToInt function test");
}

void test_NMEAParser_termcmp(void) {
    TEST_IGNORE_MESSAGE("need to implement termcmp function test");

}

void test_NMEAParser_will_decode_GPRMC(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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
    // modified tests
    // RMC packets should only influence the data herein.
    // all other data should be at default state
    //TEST_ASSERT_EQUAL
}
void test_NMEAParser_will_decode_GPGGA(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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
    // modified tests
    // GGA packets should only influence the data herein.
    // all other data should be at default state
    //TEST_ASSERT_EQUAL
}

void test_NMEAParser_will_decode_GPGSA(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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
    // modified tests
    // GSA packets should only influence the data herein.
    // all other data should be at default state
    //TEST_ASSERT_EQUAL
}

void test_NMEAParser_will_decode_GPGSV(void) {
    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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
    // modified tests
    // GSV packets should only influence the data herein.
    // all other data should be at default state
    //TEST_ASSERT_EQUAL
}

void test_NMEAParser_returns_false_unknown_sentence(void) {
    // in this function we use a GPZDA example sentence. it is assumed the code will not
    // be able to decode it. If the functionality for this sentence is added this test will fail


    // clear the values before the test to prevent interference
    NMEAParser_reset_all_values();
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