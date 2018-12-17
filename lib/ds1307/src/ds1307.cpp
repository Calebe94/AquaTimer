#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include "ds1307.h"

void ds1307_init()
{
    Wire.begin();
}

time_t get_time(void)
{
	uint8_t data[7];
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(ZERO);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 7);
    data[0] = (uint8_t)Wire.read();
    data[1] = (uint8_t)Wire.read();
    data[2] = (uint8_t)Wire.read()&0x3F;
    data[3] = (uint8_t)Wire.read();
    data[4] = (uint8_t)Wire.read();
    data[5] = (uint8_t)Wire.read();
    data[6] = (uint8_t)Wire.read();
	return (time_t)array_to_time(data);
}

void set_time(time_t time)
{
	struct tm tm;
	gmtime_r(&time, &tm);

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(ZERO);
    
    Wire.write(int_to_BCD(tm.tm_sec)&0x7F);
    Wire.write(int_to_BCD(tm.tm_min));
    Wire.write(int_to_BCD(tm.tm_hour));
    Wire.write(int_to_BCD(tm.tm_wday+1));
    Wire.write(int_to_BCD(tm.tm_mday));
    Wire.write(int_to_BCD(tm.tm_mon+1));
    Wire.write(int_to_BCD(tm.tm_year-100));
    Wire.write(ZERO);
    Wire.endTransmission();
}

time_t array_to_time(uint8_t data[])
{
	struct tm time;

	time.tm_sec  = BCD_to_int(data[0]);
	time.tm_min  = BCD_to_int(data[1]);
	time.tm_hour = BCD_to_int(data[2]);
	time.tm_mday = BCD_to_int(data[4]);
	time.tm_mon  = BCD_to_int(data[5]) - 1;
	time.tm_year = BCD_to_int(data[6]) + 100 ;
	return (time_t)mktime(&time);
}

uint8_t int_to_BCD(uint8_t integer)
{
	return ((integer/10)<<4) | (integer%10);
}

uint8_t BCD_to_int(uint8_t bcd)
{
	return ((bcd>>4)*10)+(bcd&0xF);
}

void setar_hora(uint8_t week_day, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t year)
{
  struct tm time_to_set;

  time_to_set.tm_sec = second;
  time_to_set.tm_min = minute;
  time_to_set.tm_hour = hour;
  time_to_set.tm_wday = week_day;
  time_to_set.tm_mday = day;
  time_to_set.tm_mon = month;
  time_to_set.tm_year = year - 1900;
  set_time(mktime(&time_to_set));
}
