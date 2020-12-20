/*
    NMEAParser.h - Library for parsing NMEA strings from a GPS
    Decodes GGA, GSA, GSV, and RMC strings
    Reed A. Foster, July 2017.
*/

#ifndef NMEAParser_h
#define NMEAParser_h

#include <stdbool.h>
#include <stdint.h>



#define NMEA_GGA 0
#define NMEA_GSA 1
#define NMEA_GSV 2
#define NMEA_GLL 3
#define NMEA_RMC 4
#define NMEA_VTG 5
#define NMEA_ZDA 6
#define NMEA_UNKNOWN 7

#define NMEASENTENCE_MAXLENGTH 120
#define NMEASENTENCE_MAXTERMS 25



enum {
    GPS_INVALID_DOP = 0xFFFFFFFF,       GPS_INVALID_ANGLE = 999999999,
    GPS_INVALID_ALTITUDE = 999999999,   GPS_INVALID_DATE = 0,
    GPS_INVALID_TIME = 0xFFFFFFFF,      GPS_INVALID_SPEED = 999999999,
    GPS_INVALID_FIX_TIME = 0xFFFFFFFF,  GPS_INVALID_SATELLITES = 0xFF,
    GPS_INVALID_AGE = 0xFFFFFFFF,       GPS_INVALID_SNR = 0xFFFFFFFF
};

char _sentence[NMEASENTENCE_MAXLENGTH];
int  _char_offset;


// GGA Variables
uint32_t   _time; // UTC time in hundredths of a second
int32_t        _latitude_lower, _latitude_upper; // latitude in ten millionths of a degree
int32_t            _longitude_lower, _longitude_upper; // longitude in ten millionths of a degree
uint8_t   _fixquality;
uint8_t   _numsats; // number of satellites used for fix
uint16_t  _hdop; // horizontal dilution of position (scaled by 100, i.e. 120 corresponds to a dop of 1.2)
int32_t            _altitude; // altitude in centimeters

// GSA Variables
uint8_t   _fixtype;
uint16_t  _pdop; // positional dop (same scale as hdop)
uint16_t  _vdop; // vertical dop (same scale as hdop)

// GSV Variables
uint8_t   _numsats_visible; // number of satellites visible to gps
uint8_t   _gsv_sentence, _gsv_sentences; // counter and total for gsv messages
uint8_t   _snr_count; // count of satellites with Signal to Noise Ratio
uint32_t   _snr_total, _new_snr_total; // sum of Signal to Noise Ratios (C/No, in dB) from all satellites
uint32_t   _snr_avg; // average snr of gsv message (over all sentences) scaled by 100 (i.e. 4500 corresponds to an average SNR of 45)

// RMC Variables
uint32_t   _speed; // speed in hundredths of kph
uint16_t  _course; // course in hundredths of a degree
uint16_t  _date; // UTC date

// Add variables for additional NMEA sentences here

// Other status
//uint32_t   _last_time_fix;
//uint32_t   _last_position_fix;

inline uint32_t getTime() {return _time;}
inline uint16_t getDate() {return _date;}
inline void getLatitude(int32_t *upper, int32_t *lower) {*upper = _latitude_upper; *lower = _latitude_lower;}
inline void getLongitude(int32_t *upper, int32_t *lower) {*upper = _longitude_upper; *lower = _longitude_lower;}
inline int32_t getAltitude() {return _altitude;}
inline uint16_t getPDOP() {return _pdop;}
inline uint16_t getVDOP() {return _vdop;}
inline uint16_t getHDOP() {return _hdop;}
inline uint8_t getNSats() {return _numsats;}
inline uint8_t getFixQuality() {return _fixquality;}
inline uint8_t getFixType() {return _fixtype;}
inline uint32_t getSpeed() {return _speed;}
inline uint16_t getCourse() {return _course;}
//inline uint32_t timeAge() {return millis() - _last_time_fix;}
//inline uint32_t positionAge() {return millis() - _last_position_fix;}
inline uint8_t getNSatsVisible() {return _numsats_visible;}
inline uint32_t getSNR() {return _snr_avg;}

bool NMEAParser_encode(char c);
int NMEAParser_termcmp(const char *str1, const char *str2);
int NMEAParser_hexToInt(char hex);
int32_t NMEAParser_parse_decimal(char *p);
void NMEAParser_parse_degrees(char *p, int32_t *upper, int32_t *lower);
bool NMEAParser_decode_sentence();


#endif
