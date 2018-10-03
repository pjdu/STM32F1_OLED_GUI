#include "rtc_ui.h"
#include "rotary_encorder.h"
#include "text.h"
#include "menu_ui.h"
#include "main_ui_page.h"
#include "rtc.h"
#include "config.h"
static WINDOWS RTCWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0, .title = "Time Setting"};

RTC_TimeTypeDef rtcTime;
RTC_DateTypeDef rtcDate;
static char *time;
static RTC_Mode rtcmode = RTC_Mode_Normal;
TaskHandle_t RTCUITaskHandler;
static EventBits_t val = 0;
void RTCUI_Task(void *pvParameters)
{
	int  button_press_count = 0;
	int time3s_counter = 0 ;
	_calendar_obj tmp_calendar;
	Rotary_state rstate = state_no_changed;
	GUI_ClearSCR();
	GUI_WindowsDraw(&RTCWindow);
	while(1)
	{
		if (EventGroupHandler != NULL) {
			//等待按鈕事件或長按事件發生
			val = xEventGroupWaitBits(EventGroupHandler,
					BUTTON_PRESS_EVENT | BUTTON_PRESS_1S_EVENT, //短按事件和長按事件
					pdTRUE, 	//等待完成自動清除
					pdFALSE,	//任一事件發生進入
					10 / portTICK_PERIOD_MS);
			//按鈕事件處理
			switch(val)
			{
				case BUTTON_PRESS_EVENT:
					button_press_count++;
					button_press_count %=7;
					time3s_counter = 0;
					break;
				case BUTTON_PRESS_1S_EVENT:
					GUI_ClearSCR();
					//恢復MENU UI 任務
					xTaskCreate((TaskFunction_t  )(Menu_Task),         	  	//Task Function
								(const char*     ) "Menu_Task",		      	//Task Name
								(uint16_t        ) MENU_TASK_STACK_SIZE, 	//Task Stack Size
								(void *          ) NULL,				    //Task Fuction Parameter
								(UBaseType_t     ) MENU_TASK_PRIORITY, 		//Task Priority
								(TaskHandle_t    ) &MenuTaskHandler);	    //Task Handler
					//刪除RTC UI 任務
					vTaskDelete(RTCUITaskHandler);
					break;
				case NULL_EVENT_RAISE:
					rstate = RotaryEcncorder_GetState();
					if(rstate == state_no_changed){
						time3s_counter++;
						//沒有任何按鈕事件且無旋轉持續3秒
						if(time3s_counter >= 30){
							// 30 * 100ms = 3s
							xTaskCreate((TaskFunction_t  )(main_ui_task),         	  	//Task Function
										(const char*     ) "main task",		      		//Task Name
										(uint16_t        ) MAIN_UI_PAGE_TASK_STACK_SIZE, //Task Stack Size
										(void *          ) NULL,				    	//Task Fuction Parameter
										(UBaseType_t     ) MAIN_UI_PAGE_TASK_PRIORITY, 	//Task Priority
										(TaskHandle_t    ) &mainUITaskHandler);	    	//Task Handler
							vTaskDelete(RTCUITaskHandler);
						}
					}
					break;
			}
		}
		//RTC 模式切換與設定處理
		rtcmode = (RTC_Mode)button_press_count;
		RTC_Get(); // 取得時間
		switch(rtcmode)
		{
			case RTC_Mode_Normal:

				time = pvPortMalloc(sizeof(char) * 9);
				snprintf(time, 9, "%2d:%2d:%2d", calendar.hour, calendar.min,calendar.sec);
				show_str_mid(RTCWindow.x, RTCWindow.y + 30, time, 12, 12, 1,RTCWindow.width);
				vPortFree(time);

				time = pvPortMalloc(sizeof(char) * 11);
				snprintf(time, 12, "%4d/%2d/%2d/", calendar.w_year,calendar.w_month, calendar.w_date);
				show_str_mid(RTCWindow.x, RTCWindow.y + 15, time, 12, 12, 1,RTCWindow.width);
				vPortFree(time);

//				if(HAL_RTC_GetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN) == HAL_OK){
//					time = pvPortMalloc(sizeof(char) * 9);
//					snprintf(time,9,"%2d:%2d:%2d",rtcTime.Hours,rtcTime.Minutes,rtcTime.Seconds);
//					show_str_mid(RTCWindow.x, RTCWindow.y+30, time,12,12,1,RTCWindow.width);
//					vPortFree(time);
//				}
//				if(HAL_RTC_GetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN) == HAL_OK){
//					time = pvPortMalloc(sizeof(char) * 11);
//					snprintf(time,12,"%4d/%2d/%2d/",rtcDate.Year+2000,rtcDate.Month,rtcDate.Date);
//					show_str_mid(RTCWindow.x, RTCWindow.y+15, time,12,12,1,RTCWindow.width);
//					vPortFree(time);
//				}
				break;
			case RTC_Mode_Setting_Year:
				RotaryEcncorder_SetRange(18,50);
				int32_t year = RotaryEcncorder_GetCount();
				tmp_calendar.w_year = year+2000;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",year);
				show_str(107, 15, time,12,12,0);
				vPortFree(time);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
			case RTC_Mode_Setting_Month:
				RotaryEcncorder_SetRange(1,12);
				int32_t mounth = RotaryEcncorder_GetCount();
				tmp_calendar.w_month = mounth;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",tmp_calendar.w_month);
				show_str(125, 15, time,12,12,0);
				vPortFree(time);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
			case RTC_Mode_Setting_Date:
				RotaryEcncorder_SetRange(1,31);
				int32_t date = RotaryEcncorder_GetCount();
				tmp_calendar.w_date = date;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",tmp_calendar.w_date);
				show_str(143, 15, time,12,12,0);
				vPortFree(time);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
			case RTC_Mode_Setting_Hour:
				RotaryEcncorder_SetRange(0,24);
				int32_t hour = RotaryEcncorder_GetCount();
				tmp_calendar.hour = hour;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",tmp_calendar.hour);
				show_str(104, 30, time,12,12,0);
				vPortFree(time);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
			case RTC_Mode_Setting_Minute:
				RotaryEcncorder_SetRange(0,59);
				int32_t minute = RotaryEcncorder_GetCount();
				tmp_calendar.min = minute;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",tmp_calendar.min);
				show_str(122, 30, time,12,12,0);
				vPortFree(time);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
			case RTC_Mode_Setting_Second:
				RotaryEcncorder_SetRange(0,59);
				int32_t second = RotaryEcncorder_GetCount();
				tmp_calendar.sec = second;
				time = pvPortMalloc(sizeof(char) * 3);
				snprintf(time,3,"%2d",tmp_calendar.sec);
				show_str(140, 30, time,12,12,0);
				vPortFree(time);
				//update RTC Date and Time Setting
//				HAL_RTC_SetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
//				HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
				RTC_Set(tmp_calendar.w_year,tmp_calendar.w_month,tmp_calendar.w_date,tmp_calendar.hour,tmp_calendar.min,tmp_calendar.sec);
				break;
		}
		GUI_Refresh();
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}
