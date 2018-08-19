#include "menuL1_item.h"
#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
//#include "joystick.h"
//#include "keyTask.h"
//#include "display.h"
#include "SSD1322.h"
//#include "config_param.h"
//#include "24l01.h"
//#include "menuL2_item.h"

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

#define  MAIN_MENU_ITEMNUM    		10	//主菜單項數

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
"6.搖桿校準",
"7.匹配四軸",
"8.語言選擇",
"9.重置",
"10.退出",
},
{
"1.Control Mode",
"2.Flight Mode",
"3.Flight Speed",
"4.Flip Enable",
"5.Exp Module",
"6.Joystick Calib",
"7.Match MiniFly",
"8.Language",
"9.Reset",
"10.Exit",
},
{
"1.控制模式",
"2.飛行模式",
"3.飛行速度",
"4.翻滾使能",
"5.擴展模塊",
"6.搖桿校準",
"7.匹配四軸",
"8.語言選擇",
"9.重置",
"10.退出",
},
};

void gotoJoystickCalibUI(void)
{
	exitMenu();
//	setShow_ui(JOYSTICK_CALIB_UI);
}

void gotoMatchMiniFlyUI(void)
{
	exitMenu();
//	setShow_ui(MATCH_UI);
}

void gotoResetUI(void)
{
	exitMenu();
//	setShow_ui(RESET_UI);
}

//void expModuleMenuSet(enum expModuleID id)
//{
//	if(id == NO_MODULE)
//	{
//		mainMenu[4].childrenMenu = &expModuleMenu[0];
//	}
//	else if(id == LED_RING)
//	{
//		mainMenu[4].childrenMenu = &expModuleMenu[1];
//	}
//	else if(id == WIFI_CAMERA)
//	{
//		mainMenu[4].childrenMenu = &expModuleMenu[2];
//	}
//	else if(id == OPTICAL_FLOW)
//	{
//		mainMenu[4].childrenMenu = &expModuleMenu[3];
//	}
//}

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
	mainMenu[0].Function = gotoNextMenu;
	//mainMenu[0].childrenMenu = controlmodeMenu;

	mainMenu[1].Function = gotoNextMenu;
	//mainMenu[1].childrenMenu = flymodeMenu;
	
	mainMenu[2].Function = gotoNextMenu;
	//mainMenu[2].childrenMenu = flyspeedMenu;
	
	mainMenu[3].Function = gotoNextMenu;
	//mainMenu[3].childrenMenu = flipEnableMenu;
	
	mainMenu[4].Function = gotoNextMenu;
	//mainMenu[4].childrenMenu = &expModuleMenu[0];
	
	mainMenu[5].Function = gotoJoystickCalibUI;
	//mainMenu[5].childrenMenu = NULL;
	
	mainMenu[6].Function = gotoMatchMiniFlyUI;
	//mainMenu[6].childrenMenu = NULL;
	
	mainMenu[7].Function = gotoNextMenu;
	//mainMenu[7].childrenMenu = languageMenu;
	
	mainMenu[8].Function = gotoResetUI;
	//mainMenu[8].childrenMenu = NULL;
	
	mainMenu[9].Function = exitMenu;
	//mainMenu[9].childrenMenu = NULL;
}
