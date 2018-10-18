#include "sensor_ui_template.h"
#include "air_data.h"
#include "rtc_ui.h"
#include "decode_command.h"
#include "rotary_encorder.h"
#include "main_ui_page.h"
#include "rtc.h"

TaskHandle_t SensorUITaskHandler;
static WINDOWS SensorWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0};

static EventBits_t val = 0;
uint8_t *title_buf;
static uint8_t *buf;


static Device_Msg data;

void SENSOR_UI_Task(void *pvParameters)
{
	int rotatnum = 0;
	int time_counter = 0 ;
	int limit_page_display_num;
	Rotary_state rstate = state_no_changed;

	GUI_ClearSCR();
	GUI_WindowsDraw(&SensorWindow);
	RotaryEcncorder_SetRange(0,CONFIG_MAX_DEVICE_SIZE);
	RotaryEcncorder_SetCount(0);
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
				case BUTTON_PRESS_1S_EVENT:
					//返回前一頁
					GUI_ClearSCR();
					//恢復MENU UI 任務
					xTaskCreate((TaskFunction_t  )(Menu_Task),         	  	//Task Function
								(const char*     ) "Menu_Task",		      	//Task Name
								(uint16_t        ) MENU_TASK_STACK_SIZE, 	//Task Stack Size
								(void *          ) NULL,				    //Task Fuction Parameter
								(UBaseType_t     ) MENU_TASK_PRIORITY, 		//Task Priority
								(TaskHandle_t*    ) &MenuTaskHandler);	    //Task Handler
					//刪除Sensor UI 任務
					vTaskDelete(SensorUITaskHandler);
					break;
				case NULL_EVENT_RAISE:
					rstate = RotaryEcncorder_GetState();
					if(rstate == state_no_changed){
						time_counter++;
						if(time_counter > 200){
						//delay 300ms * 200 ~= 1minute
						xTaskCreate((TaskFunction_t  )(main_ui_task),         	  	//Task Function
									(const char*     ) "main task",		      		//Task Name
									(uint16_t        ) MAIN_UI_PAGE_TASK_STACK_SIZE, //Task Stack Size
									(void *          ) NULL,				    	//Task Fuction Parameter
									(UBaseType_t     ) MAIN_UI_PAGE_TASK_PRIORITY, 	//Task Priority
									(TaskHandle_t*    ) &mainUITaskHandler);	    	//Task Handler
						vTaskDelete(SensorUITaskHandler);
						}
					}
					break;
			}
		}

		limit_page_display_num = airdata_get_numberofDevice(0);
		limit_page_display_num--;
		RotaryEcncorder_SetRange(0,limit_page_display_num);
		rotatnum = RotaryEcncorder_GetCount();

		//限制旋轉值小於等於當前可顯示頁面
		rotatnum = (rotatnum > limit_page_display_num)?limit_page_display_num:rotatnum;

		//顯示Sensor 標題頁名稱
		SensorWindow.title = pvPortMalloc(sizeof(uint8_t) * 16);
		snprintf(SensorWindow.title, 14, "Device %d view", rotatnum+1);
		show_str_mid(SensorWindow.x, SensorWindow.y + 1, SensorWindow.title, 12,12, 0, SensorWindow.width);
		vPortFree(SensorWindow.title);

		// get airdata
		data.shortaddress = airdata_get_shortaddress(rotatnum);//getShortaddress(rotatnum);
		data.temperature = airdata_get_temperature(rotatnum);
		data.humidity = airdata_get_humidity(rotatnum);
		data.co2 = airdata_get_co2(rotatnum);
		data.voc = airdata_get_voc(rotatnum);;
		data.pm2_5 = airdata_get_pm2_5(rotatnum);




		//安排螢幕資料顯示以及位置
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
		tmp = data.co2;
		snprintf(buf, 10, "CO2 :%3f", tmp);
		show_str(SensorWindow.x + 85, SensorWindow.y + 15, buf, 12, 12, 1);
		vPortFree(buf);
		//VOC
		buf = pvPortMalloc(sizeof(char) * 11);
		tmp = data.voc / 1000.;
		snprintf(buf, 11, "VOC :%.3f", tmp);
		show_str(SensorWindow.x + 85, SensorWindow.y + 30, buf, 12, 12, 1);
		vPortFree(buf);

		//TIME
		buf = pvPortMalloc(sizeof(char) * 25);

		RTC_Get();
		snprintf(buf, 25, "TIME:%4d/%2d/%2d-%2d:%2d:%2d", calendar.w_year,calendar.w_month,
														  calendar.w_date,calendar.hour,
														  calendar.min,calendar.sec);
		show_str(SensorWindow.x + 85, SensorWindow.y + 45, buf, 12, 12, 1);
		vPortFree(buf);

		//PM2_5
		buf = pvPortMalloc(sizeof(char) * 10);
		tmp = data.pm2_5 / 10.;
		snprintf(buf, 10, "PM25:%2.1f", tmp);
		show_str(SensorWindow.x + 185, SensorWindow.y + 15, buf, 12, 12, 1);
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

		// 更新畫面顯示
		GUI_Refresh();
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
