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

#ifndef SKYTRAQ_BINARY_TYPES_H
#define SKYTRAQ_BINARY_TYPES_H

typedef enum SYSTEM_INPUT_IDs {
  SYSTEM_RESTART                           = 0x1,    // perform a reset
  QUERY_SOFTWARE_VERSION                   = 0x2,    // reset pointer to log buffer
  QUERY_SOFTWARE_CRC                       = 0x3,
  SET_FACTORY_DEFAULTS                     = 0x4,
  CONFIGURE_SERIAL_PORT                    = 0x5,
  CONFIGURE_NMEA                           = 0x8,
  CONFIGURE_MESSAGE_TYPE                   = 0x9,
  UPLOAD_SOFTWARE_IMAGE                    = 0xB,
  CONFIGURE_POWER_MODE                     = 0xC,
  CONFIGURE_UPDATE_RATE                    = 0xE,
  QUERY_UPDATE_RATE                        = 0x10,
  CONFIGURE_NAV_MESSAGE_INTERVAL           = 0x11,
  QUERY_POWER_MODE                         = 0x15

} SYSTEM_INPUT_IDs;

typedef enum GNSS_INPUT_IDs {
  CONFIGURE_DATUM                          = 0x29,
  CONFIGURE_DOP_MASK                       = 0x2A,
  CONFIGURE_ELEVATION_CNR_MASK             = 0x2B,
  QUERY_DATUM                              = 0x2D,
  QUERY_DOP_MASK                           = 0x2E,
  QUERY_ELEVATION_CNR_MASK                 = 0x2F,
  GET_GPS_EPHEMERIS                        = 0x30,
  CONFIGURE_POSITION_PINNING               = 0x39,
  QUERY_POSITION_PINNING                   = 0x3A,
  CONFIGURE_POSITION_PINNING_PARAMETERS    = 0x3B,
  SET_GPS_EPHEMERIS                        = 0x41,
  QUERY_1PPS_TIMING                        = 0x44,
  CONFIGURE_1PPS_CABLE_DELAY               = 0X45,
  QUERY_1PPS_CABLE_DELAY                   = 0x46,
  CONFIGURE_NMEA_TALKER_ID                 = 0x4B,
  QUERY_NMEA_TALKER_ID                     = 0x4F,
  GET_GPS_ALMANAC                          = 0x50,
  SET_GPS_ALMANAC                          = 0x51,
  CONFIGURE_1PPS_TIMING                    = 0x54,
  GET_GLONASS_EPHEMERIS                    = 0x5B,
  SET_GLONASS_EPHEMERIS                    = 0x5C,
  GET_GLONASS_ALMANAC                      = 0x5D,
  SET_GLONASS_ALMANAC                      = 0x5E,
  GET_GLONASS_TIME_CORRECT_                = 0x5F,
  SET_GLONASS_TIME_CORRECT                 = 0x60,
}




#endif /* SKYTRAQ_BINARY_TYPES_H */
