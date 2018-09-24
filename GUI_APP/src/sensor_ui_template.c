#include "sensor_ui_template.h"
#include "air_data.h"
#include "rtc_ui.h"
#include "decode_command.h"


TaskHandle_t SensorUITaskHandler;
static WINDOWS SensorWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0};

static EventBits_t val = 0;
uint8_t *title_buf;
static uint8_t *buf;


static Device_Msg data;

void SENSOR_UI_Task(void *pvParameters)
{
	int32_t rotatnum = 0;

	GUI_ClearSCR();
	GUI_WindowsDraw(&SensorWindow);
	RotaryEcncorder_SetRange(1,CONFIG_MAX_DEVICE_SIZE);
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
			if(val == BUTTON_PRESS_1S_EVENT){
				//返回前一頁
				GUI_ClearSCR();
				//恢復MENU UI 任務
				xTaskCreate((TaskFunction_t  )(Menu_Task),         	  	//Task Function
							(const char*     ) "Menu_Task",		      	//Task Name
							(uint16_t        ) MENU_TASK_STACK_SIZE, 	//Task Stack Size
							(void *          ) NULL,				    //Task Fuction Parameter
							(UBaseType_t     ) MENU_TASK_PRIORITY, 		//Task Priority
							(TaskHandle_t    ) &MenuTaskHandler);	    //Task Handler
				//刪除RTC UI 任務
				vTaskDelete(SensorUITaskHandler);
			}
		}

		rotatnum = RotaryEcncorder_GetCount();

		//顯示Sensor 標題頁名稱
		SensorWindow.title = pvPortMalloc(sizeof(uint8_t) * 16);
		snprintf(SensorWindow.title, 14, "Sensor %d view", rotatnum);
		show_str_mid(SensorWindow.x, SensorWindow.y + 1, SensorWindow.title, 12,12, 0, SensorWindow.width);
		vPortFree(SensorWindow.title);

		rotatnum = rotatnum - 1;
		data.shortaddress = getShortaddress(rotatnum);
		data.temperature = getTemperature(rotatnum);
		data.humidity = getHumdity(rotatnum);
		data.co2 = getCo2(rotatnum);
		data.voc = getVoc(rotatnum);
		data.pm2_5 = getPM2_5(rotatnum);
//		data.IAQMode = getIAQMode(rotatnum);
//		data.fan1Mode = getFan1Mode(rotatnum);
//		data.fan2Mode = getFan2Mode(rotatnum);

		double tmp;
		//Temperature
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.temperature / 10.;
		snprintf(buf, 10, "Temp:%2.1f", tmp);
		show_str(SensorWindow.x + 5, SensorWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//Humidity
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.humidity / 10.;
		snprintf(buf, 10, "Humi:%2.1f", tmp);
		show_str(SensorWindow.x + 5, SensorWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

		//addr
		buf = pvPortMalloc(sizeof(char) * 10);
		snprintf(buf, 10, "Addr:%4X", data.shortaddress);
		show_str(SensorWindow.x + 5, SensorWindow.y + 45, buf, 12, 12, 1);
		vPortFree(buf);

		//CO2
		buf = pvPortMalloc(sizeof(char) * 10);
		snprintf(buf, 10, "CO2 :%3d", data.co2);
		show_str(SensorWindow.x + 65, SensorWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//VOC
		buf = pvPortMalloc(sizeof(char) * 11);
		tmp = data.voc / 1000.;
		snprintf(buf, 11, "VOC :%.3f", tmp);
		show_str(SensorWindow.x + 65, SensorWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

		//TIME
		buf = pvPortMalloc(sizeof(char) * 25);
		HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
		snprintf(buf, 25, "TIME:%4d/%2d/%2d-%2d:%2d:%2d", rtcDate.Year + 2000,
				rtcDate.Month, rtcDate.Date, rtcTime.Hours, rtcTime.Minutes,
				rtcTime.Seconds);
		show_str(SensorWindow.x + 65, SensorWindow.y + 45, buf, 12, 12, 1);
		vPortFree(buf);

		//PM2_5
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.pm2_5 / 10.;
		snprintf(buf, 10, "PM25:%2.1f", tmp);
		show_str(SensorWindow.x + 125, SensorWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//IAQ
//		buf = pvPortMalloc(sizeof(char) * 9);
//		snprintf(buf, 9, "IAQ :%3f", data.IAQMode);
//		show_str(SensorWindow.x + 125, SensorWindow.y + 30, buf, 12, 12, 1);
//		vPortFree(buf);
//		//FAN1
//		buf = pvPortMalloc(sizeof(char) * 9);
//		snprintf(buf, 9, "FAN1:%3f", data.fan1Mode);
//		show_str(SensorWindow.x + 185, SensorWindow.y + 15, buf, 12, 12, 1);
//		vPortFree(buf);
//		//FAN2
//		buf = pvPortMalloc(sizeof(char) * 9);
//		snprintf(buf, 9, "FAN2:%3f", data.fan2Mode);
//		show_str(SensorWindow.x + 185, SensorWindow.y + 30, buf, 12, 12, 1);
//		vPortFree(buf);

		GUI_Refresh();
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
