#ifndef __RTC_UI_H
#define __RTC_UI_H

#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "messagebox.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

typedef enum {
	RTC_Mode_Normal = 0,
	RTC_Mode_Setting_Year,
	RTC_Mode_Setting_Month,
	RTC_Mode_Setting_Date,
	RTC_Mode_Setting_Hour,
	RTC_Mode_Setting_Minute,
	RTC_Mode_Setting_Second

} RTC_Mode;


extern RTC_HandleTypeDef hrtc;

#define RTCUI_TASK_PRIORITY   				3  		// Task Priority
#define RTCUI_TASK_STACK_SIZE 				256		// Task Stack Size

#define RTC_POSX_1 20
#define RTC_POSY_1 19

extern TaskHandle_t RTCUITaskHandler;						// Task Handler
void RTCUI_Task(void *pvParameters);


#endif
