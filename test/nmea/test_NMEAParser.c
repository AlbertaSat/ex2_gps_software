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
            TEST_ASSERT_EQUAL(true, new_data);
        } else {
            TEST_ASSERT_EQUAL(false, new_data);
        }
        
    }
}
