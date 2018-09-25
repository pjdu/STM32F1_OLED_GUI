#ifndef __MAIN_UI_PAGE_H
#define __MAIN_UI_PAGE_H

#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "messagebox.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"



/*Menu_Task==========================================*/
#define MAIN_UI_PAGE_TASK_PRIORITY   		2  		// Task Priority
#define MAIN_UI_PAGE_TASK_STACK_SIZE 		256		// Task Stack Size
extern TaskHandle_t mainUITaskHandler;				// Task Handler


void main_ui_task(void *pvparameter);

#endif
