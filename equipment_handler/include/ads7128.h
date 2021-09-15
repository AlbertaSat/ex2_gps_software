/*
 * Copyright (C) 2020  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/**
 * @file ads7128.h
 * @author Josh Lazaruk
 * @date 2021-09-10
 */
#include <stdint.h>

#define ADS7128_ADDR              (0x11)
#ifdef IS_ATHENA
#define ADS7128_PORT              i2cREG2
#else
#define ADS7128_PORT              i2cREG1 // port used on dev board
#endif

void voltageToTemperature(uint16_t voltage, int* temperature);
uint8_t readSingleTemp(uint8_t channel, int* temperature);
uint8_t readAllTemps(int* temperatures);