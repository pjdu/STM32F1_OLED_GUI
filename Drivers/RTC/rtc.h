#ifndef __RTC__H
#define __RTC__H
#include "stm32f1xx_hal.h"

extern RTC_HandleTypeDef hrtc;  //rtc struct handler in main.c

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_date;
	uint8_t  week;
}_calendar_obj;

extern _calendar_obj calendar;
void MY_RTC_Init(void);
HAL_StatusTypeDef RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
void RTC_Get(void);

void MY_RTC_Init(void);
HAL_StatusTypeDef RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
void RTC_Get(void);


#endif
