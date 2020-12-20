#include "unity.h"

#include "NMEAParser.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}


void test_NMEAParser_decode_sentence_with_GPRMC(void) {
    char *NMEA_to_decode = "$GPRMC,165401.997831,A,3725.0690,N,12203.3824,W,0.000,082.1,070717,,,A*48\r\n";
    strcpy(_sentence, NMEA_to_decode);
    NMEAParser_decode_sentence();
    //TEST_ASSERT_EQUAL_INT32()
}


void test_NMEAParser_NeedToImplement(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement NMEAParser");
}
