#include "main_ui_page.h"
#include "menu_ui.h"
#include "config.h"
#include "air_data.h"
#include "rtc_ui.h"

TaskHandle_t mainUITaskHandler;
static WINDOWS mainUIWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0, .title ="IAQ Weather Main Page"};

extern EventGroupHandle_t EventGroupHandler;
static EventBits_t val = 0;
static Device_Msg data;

void main_ui_task(void *pvparameter)
{
	GUI_ClearSCR();
	GUI_WindowsDraw(&mainUIWindow);
	uint8_t *buf;
	double tmp;
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
			switch(val){
				case BUTTON_PRESS_EVENT:
					GUI_ClearSCR();
					//進入選單頁面
					//創建MENU UI 任務
					xTaskCreate((TaskFunction_t  )(Menu_Task),         	  	//Task Function
								(const char*     ) "Menu_Task",		      	//Task Name
								(uint16_t        ) MENU_TASK_STACK_SIZE, 	//Task Stack Size
								(void *          ) NULL,				    //Task Fuction Parameter
								(UBaseType_t     ) MENU_TASK_PRIORITY, 		//Task Priority
								(TaskHandle_t    ) &MenuTaskHandler);	    //Task Handler
					//刪除RTC UI 任務
					vTaskDelete(mainUITaskHandler);
					break;
				case NULL_EVENT_RAISE:
					break;
			}

		}

		data.numberofDevice = getNumberofDevice(0);
		if(data.numberofDevice != 0)
		{
			data.temperature = 0;
			data.humidity = 0;
			data.co2 = 0;
			data.voc = 0;
			data.pm2_5 = 0;

			for(int i = 0 ; i< data.numberofDevice; i ++)
			{
				data.temperature += getTemperature(i);
				data.humidity += getHumdity(i);
				data.co2 += getCo2(i);
				data.voc += getVoc(i);
				data.pm2_5 += getPM2_5(i);
			}
		}

		//Temperature
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.temperature / 10.;
		snprintf(buf, 10, "Temp:%2.1f", tmp / data.numberofDevice);
		show_str(mainUIWindow.x + 5, mainUIWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//Humidity
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.humidity / 10.;
		snprintf(buf, 10, "Humi:%2.1f", tmp / data.numberofDevice);
		show_str(mainUIWindow.x + 5, mainUIWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

//		//addr
//		buf = pvPortMalloc(sizeof(char) * 10);
//		snprintf(buf, 10, "Addr:%4X", data.shortaddress);
//		show_str(mainUIWindow.x + 5, mainUIWindow.y + 45, buf, 12, 12, 1);
//		vPortFree(buf);

		//CO2
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.co2;
		snprintf(buf, 10, "CO2 :%3f", tmp / data.numberofDevice );
		show_str(mainUIWindow.x + 65, mainUIWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//VOC
		buf = pvPortMalloc(sizeof(char) * 11);
		tmp = data.voc / 1000.;
		snprintf(buf, 11, "VOC :%.3f", tmp / data.numberofDevice);
		show_str(mainUIWindow.x + 65, mainUIWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

		//TIME
		buf = pvPortMalloc(sizeof(char) * 25);
		HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
		snprintf(buf, 25, "TIME:%4d/%2d/%2d-%2d:%2d:%2d", rtcDate.Year + 2000,
														  rtcDate.Month,
														  rtcDate.Date,
														  rtcTime.Hours,
														  rtcTime.Minutes,
														  rtcTime.Seconds);
		show_str(mainUIWindow.x + 65, mainUIWindow.y + 45, buf, 12, 12, 1);
		vPortFree(buf);

		//PM2_5
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.pm2_5 / 10.;
		snprintf(buf, 10, "PM25:%2.1f", tmp / data.numberofDevice);
		show_str(mainUIWindow.x + 125, mainUIWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);

		//deviceNumber
		buf = pvPortMalloc(sizeof(char) * 10);
		snprintf(buf, 10, "dev :%d", data.numberofDevice);
		show_str(mainUIWindow.x + 125, mainUIWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

		GUI_Refresh();
		vTaskDelay(100/portTICK_PERIOD_MS);
	}

}
