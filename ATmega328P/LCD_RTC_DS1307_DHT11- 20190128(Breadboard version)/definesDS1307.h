/*
 * definesDS1307.h
 *
 * Created: 2017-07-16 오후 4:36:53
 *  Author: insoo
 */ 


#ifndef DEFINESDS1307_H_
#define DEFINESDS1307_H_

// ---------------------------------------------------------------------------
// DS1307 RTC ROUTINES
#define DS1307 0xD0 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F

#define AM 0
#define PM 1




#endif /* DEFINESDS1307_H_ */