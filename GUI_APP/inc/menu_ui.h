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

/********************************************************************************	 
 * 本程序只供學習使用，未經作者許可，不得用於其它任何用途
 * ALIENTEK MiniFly_Remotor
 * 菜單顯示界面代碼
 * 正點原子@ALIENTEK
 * 技術論壇:www.openedv.com
 * 創建日期:2018/6/1
 * 版本：V1.0
 * 版權所有，盜版必究。
 * Copyright(C) 廣州市星翼電子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

extern WINDOWS MenuWindow;
extern Scrollbar_Typedef MenuScrollbar;

/*Menu_Task==========================================*/
#define MENU_TASK_PRIORITY   				2  		// Task Priority
#define MENU_TASK_STACK_SIZE 				128		// Task Stack Size
extern TaskHandle_t MenuTaskHandler;						// Task Handler


void Button_task(void *pvParameters);
void menu_test(void);
void Menu_Task(void *pvParameters);//運行菜單
void MenuHookFun(uint8_t key);//鉤子函數
void exitMenu(void);
void gotoNextMenu(void);
void gotoLastMenu(void);


#endif /*__GUI_MENU_H*/
