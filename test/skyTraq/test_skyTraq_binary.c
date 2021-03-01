#include "unity.h"

#include "skyTraq_binary.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_skytraq_checksum_will_pass(void) {
    // configure nmea message interval example
    uint8_t message_to_decode[] = {0xA0, 0xA1, 0x00, 0x09, 0x08, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x0D, 0x0A};
    bool result = skytraq_verify_checksum(message_to_decode, 9, 8);
    TEST_ASSERT_EQUAL(true,result);
}

void test_skytraq_checksum_will_fail(void) {
    uint8_t message_to_decode[] = {0xA0, 0xA1, 0x00, 0x09, 0x08, 0x01, 0x01, 0x01, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x08, 0x0D, 0x0A};
    bool result = skytraq_verify_checksum(message_to_decode, 9, 8);
    TEST_ASSERT_EQUAL(false,result);
}