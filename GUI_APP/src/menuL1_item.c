#include "menuL1_item.h"
#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "SSD1322.h"
#include "rtc_ui.h"
#include "sensor_ui_template.h"

/********************************************************************************	 
 * 本程序只供學習使用，未經作者許可，不得用於其它任何用途
 * ALIENTEK MiniFly_Remotor
 * 一級菜單驅動代碼
 * 正點原子@ALIENTEK
 * 技術論壇:www.openedv.com
 * 創建日期:2018/6/1
 * 版本：V1.0
 * 版權所有，盜版必究。
 * Copyright(C) 廣州市星翼電子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

#define  MAIN_MENU_ITEMNUM    		5	//主菜單項數

MenuItem_Typedef  mainMenu[MAIN_MENU_ITEMNUM];

/****** 手指按鈕圖標 陰碼 逐列式 順向(高位在前) *****/
const uint8_t finger_img[32]={
0x07,0xF0,0x04,0x10,0x04,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x04,0x20,0x04,
0x41,0x54,0x4D,0x5C,0x75,0x70,0x05,0xC0,0x05,0x00,0x05,0x00,0x05,0x00,0x02,0x00,
};

const uint8_t mainMenuTitle[] = "ATK MiniFly";

const char* mainMenuLanguage[3][MAIN_MENU_ITEMNUM]={
{
"1.控制模式",
"2.飛行模式",
"3.飛行速度",
"4.翻滾使能",
"5.擴展模塊",
},
{
"1.Time Setting",
"2.Sensor 1 State",
"3.Sensor 2 State",
"4.Sensor 3 State",
"5.Sensor 4 State",
},
{
"1.控制模式",
"2.飛行模式",
"3.飛行速度",
"4.翻滾使能",
"5.擴展模塊",
},
};

static void gotoTimeSettingUI(void)
{
	exitMenu();
	xTaskCreate((TaskFunction_t  )(RTCUI_Task),         	//Task Function
				(const char*     ) "RTCUI_Task",		    //Task Name
				(uint16_t        ) RTCUI_TASK_STACK_SIZE, 	//Task Stack Size
				(void *          ) NULL,				    //Task Fuction Parameter
				(UBaseType_t     ) RTCUI_TASK_PRIORITY, 	//Task Priority
				(TaskHandle_t    ) &RTCUITaskHandler);	    //Task Handler

	vTaskDelete(MenuTaskHandler);
}

static void gotoSensorUI(void)
{
	exitMenu();
	xTaskCreate((TaskFunction_t  )(SENSOR_UI_Task),         	//Task Function
				(const char*     ) "SENSOR_UI_Task",		    //Task Name
				(uint16_t        ) SENSOR_UI_Task_STACK_SIZE, 	//Task Stack Size
				(void *          ) &(cur_rotateNum),			//Task Fuction Parameter
				(UBaseType_t     ) SENSOR_UI_Task_PRIORITY, 	//Task Priority
				(TaskHandle_t    ) &SensorUITaskHandler);	    //Task Handler
	vTaskDelete(MenuTaskHandler);
}

/*初始化主菜單*/
void mainMenuInit(void)
{	
	for(int i=0; i<MAIN_MENU_ITEMNUM; i++)
	{
		mainMenu[i].menuItemCount = MAIN_MENU_ITEMNUM;
		mainMenu[i].isSelect = false;
		mainMenu[i].icoSelected = finger_img;
		mainMenu[i].icoUnselected = NULL;
		mainMenu[i].title = mainMenuLanguage[1][i];
		mainMenu[i].Function = NULL;
		mainMenu[i].parentMenu = NULL;
		mainMenu[i].childrenMenu = NULL;
	}
	mainMenu[0].isSelect = true;
	mainMenu[0].Function = gotoTimeSettingUI;
	mainMenu[0].childrenMenu = NULL;

	mainMenu[1].Function = gotoSensorUI;
	mainMenu[1].childrenMenu = NULL;
	
	mainMenu[2].Function = gotoSensorUI;
	mainMenu[2].childrenMenu = NULL;

	mainMenu[3].Function = gotoSensorUI;
	mainMenu[3].childrenMenu = NULL;
	
	
	mainMenu[4].Function = gotoSensorUI;
	mainMenu[4].childrenMenu = NULL;
	
//	mainMenu[5].Function = gotoJoystickCalibUI;
	//mainMenu[5].childrenMenu = NULL;
	
//	mainMenu[6].Function = gotoMatchMiniFlyUI;
	//mainMenu[6].childrenMenu = NULL;
	
//	mainMenu[7].Function = gotoNextMenu;
	//mainMenu[7].childrenMenu = languageMenu;
	
//	mainMenu[8].Function = gotoResetUI;
	//mainMenu[8].childrenMenu = NULL;
	
//	mainMenu[9].Function = exitMenu;
	//mainMenu[9].childrenMenu = NULL;
}
