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
#include "config.h"


/*Menu_Task==========================================*/

#define MAIN_UI_PAGE_TASK_PRIORITY   		CONFIG_MAIN_UI_PAGE_TASK_PRIORITY  		// Task Priority
#define MAIN_UI_PAGE_TASK_STACK_SIZE 		CONFIG_MAIN_UI_PAGE_TASK_STACK_SIZE		// Task Stack Size
extern TaskHandle_t mainUITaskHandler;				// Task Handler


void main_ui_task(void *pvparameter);

#endif
