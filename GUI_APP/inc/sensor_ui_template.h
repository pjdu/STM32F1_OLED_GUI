#ifndef __SENSOR_UI_TEMPLATE_H
#define __SENSOR_UI_TEMPLATE_H

#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "messagebox.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "menu_ui.h"

#define SENSOR_UI_Task_PRIORITY   				3  		// Task Priority
#define SENSOR_UI_Task_STACK_SIZE 				256		// Task Stack Size

extern EventGroupHandle_t EventGroupHandler;
extern TaskHandle_t SensorUITaskHandler;						// Task Handler

void SENSOR_UI_Task(void *pvParameters);



#endif
