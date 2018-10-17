#ifndef __GUI_MENU_H
#define __GUI_MENU_H
#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "scrollbar.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "config.h"

extern WINDOWS MenuWindow;
extern Scrollbar_Typedef MenuScrollbar;
extern int32_t cur_rotateNum;
/*Menu_Task==========================================*/

#define MENU_TASK_PRIORITY   	CONFIG_MENU_TASK_PRIORITY  		// Task Priority
#define MENU_TASK_STACK_SIZE 	CONFIG_MENU_TASK_STACK_SIZE		// Task Stack Size

extern TaskHandle_t MenuTaskHandler;						// Task Handler


void Button_task(void *pvParameters);
void menu_test(void);
void Menu_Task(void *pvParameters);//運行菜單
void MenuHookFun(uint8_t key);//鉤子函數
void exitMenu(void);
void gotoNextMenu(void);
void gotoLastMenu(void);


#endif /*__GUI_MENU_H*/
