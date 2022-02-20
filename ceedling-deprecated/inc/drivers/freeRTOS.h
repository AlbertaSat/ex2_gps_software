/* This file's purpose is purely to have one place that
   contains every freeRTOS API call this driver is to use
   This is to make it easy to mock the calls for cmock.
   This file will be removed once the code is being integrated
   with the obc
*/

#ifndef FREERTOS_H
#define FREERTOS_H

#include <stdint.h>

typedef uint32_t TickType_t;

TickType_t xTaskGetTickCountFromISR(void);
TickType_t xTaskGetTickCount(void);

#endif // FREERTOS_H