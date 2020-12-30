#ifndef FREERTOS_H
#define FREERTOS_H

#include <stdint.h>

typedef uint32_t TickType_t;

TickType_t xTaskGetTickCountFromISR( void );
TickType_t xTaskGetTickCount( void );

#endif // FREERTOS_H