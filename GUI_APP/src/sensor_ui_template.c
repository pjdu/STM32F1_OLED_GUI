#include "sensor_ui_template.h"
#include "air_data.h"
TaskHandle_t SensorUITaskHandler;
static WINDOWS SensorWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0};

static EventBits_t val = 0;
uint8_t *title_buf;
static uint8_t *buf;
int32_t view = 0;
static air_data_packet data;
void SENSOR_UI_Task(void *pvParameters)
{
	GUI_ClearSCR();
	SensorWindow.title = pvPortMalloc(sizeof(uint8_t) * 14);
	view = *((int32_t *)pvParameters);
	snprintf(SensorWindow.title ,14,"Sensor %d view",view);
	GUI_WindowsDraw(&SensorWindow);
	vPortFree(SensorWindow.title);
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
		SENSOR_UI_VIEW sview = (SENSOR_UI_VIEW)view;
		switch(sview){
			case SENSOR_UI_VIEW_PAGE1:
			case SENSOR_UI_VIEW_PAGE2:
			case SENSOR_UI_VIEW_PAGE3:
			case SENSOR_UI_VIEW_PAGE4:

				data.temperature = getTemperature();
				data.humidity = getHumdity();
				data.co2 = getCo2();
				data.voc = getVoc();
				data.pm2_5 = getPM2_5();
				data.IAQMode = getIAQMode();
				data.fan1Mode = getFan1Mode();
				data.fan2Mode = getFan2Mode();

				buf= pvPortMalloc(sizeof(char) * 9);
				snprintf(buf,9,"Temp:%3f",data.temperature);
				show_str(SensorWindow.x+5, SensorWindow.y+15,buf,12,12,1);
				vPortFree(buf);

				buf= pvPortMalloc(sizeof(char) * 9);
				snprintf(buf,9,"Humi:%3f",data.humidity);
				show_str(SensorWindow.x+5, SensorWindow.y+30,buf,12,12,1);
				vPortFree(buf);

				buf= pvPortMalloc(sizeof(char) * 9);
				snprintf(buf,9,"CO2 :%3f",data.temperature);
				show_str(SensorWindow.x+5, SensorWindow.y+45,buf,12,12,1);
				vPortFree(buf);

				break;
		}
		GUI_Refresh();
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
