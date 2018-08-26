#include "rtc_ui.h"
#include "rotary_encorder.h"
#include "text.h"
#include "menu_ui.h"
WINDOWS RTCWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0, .title = "Time Setting"};

RTC_TimeTypeDef rtcTime;
RTC_DateTypeDef rtcDate;
char *time;
char *date;

TaskHandle_t RTCUITaskHandler;

void RTCUI_Task(void *pvParameters)
{
	GUI_ClearSCR();
	GUI_WindowsDraw(&RTCWindow);
	while(1)
	{
//		if (EventGroupHandler != NULL) {
//			EventBits_t val;
//			val = xEventGroupWaitBits(EventGroupHandler,
//									  BUTTON_PRESS_EVENT,
//									  pdTRUE,
//									  pdFALSE, 10 / portTICK_PERIOD_MS);
//			if (val == BUTTON_PRESS_EVENT) {
//				vTaskSuspend(RTCUITaskHandler);
//				vTaskResume(MenuTaskHandler);
//			}
//		}

		if(HAL_RTC_GetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN) == HAL_OK){
			time = pvPortMalloc(sizeof(char) * 9);
			snprintf(time,9,"%2d:%2d:%2d",rtcTime.Hours,rtcTime.Minutes,rtcTime.Seconds);
			show_str_mid(RTCWindow.x, RTCWindow.y+30, time,12,12,1,RTCWindow.width);
			vPortFree(time);
		}
		if(HAL_RTC_GetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN) == HAL_OK){
			date = pvPortMalloc(sizeof(char) * 11);
			snprintf(date,11,"%4d/%2d/%2d/",rtcDate.Year+2000,rtcDate.Month,rtcDate.Date);
			show_str_mid(RTCWindow.x, RTCWindow.y+15, date,12,12,1,RTCWindow.width);
			vPortFree(date);
		}


		GUI_Refresh();
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}
