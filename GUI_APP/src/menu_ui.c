#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "rotary_encorder.h"
//#include "display.h
#include "menuL1_item.h"
//#include "menuL2_item.h"

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

//菜單的位置
#define   MEMU_POSX_1    20
#define   MEMU_POSY_1    19

static MenuItem_Typedef* CurMenu = mainMenu;  //當前Menu的控制元件
static MenuItem_Typedef* CurItem;

static int32_t cur_rotateNum=0;
static int selected;
static int cur_sequence = 0; //某一級菜單選中項目的序號
static bool isChangeMenu = true; //menu狀態是否改變
const uint8_t* defaultTitle = "ATK MiniFly";

//窗體
WINDOWS MenuWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0, .title = "IAQ Weather"};

//主窗體滾動條
Scrollbar_Typedef MenuScrollbar = { .x = 245, .y = 14, .width = 10,
		.height = 50, .itemsperpage = 3, .topitem = 0, .scbbarlen = 0, };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  從menu清單中取得被選中的Menu項目
//  返回值 : 被選中的menu項目位於清單中的索引
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
uint8_t getMenuSelectitem(MenuItem_Typedef menu[]) {
	int i;
	for (i = 0; i < menu->menuItemCount; i++) {
		if (menu[i].isSelect == true)
			break;
	}
	return i;
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  返回主menu選單
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void exitMenu(void) {
	CurMenu = mainMenu;
	MenuWindow.title = defaultTitle;
	//setShow_ui(MAIN_UI);
	isChangeMenu = true;
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  前往下一個選單
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void gotoNextMenu(void) {
	if (CurItem->childrenMenu != NULL) {
		CurMenu = CurItem->childrenMenu;
		MenuWindow.title = CurItem->title + 2;
		isChangeMenu = true;
	} else if (CurItem->Function != NULL && CurItem->parentMenu == NULL) {
		CurItem->Function(); //執行選中子選單函數
	}
}

void gotoLastMenu(void) {
	if (CurItem->parentMenu != NULL) {
		//恢復默認選中
		CurItem->isSelect = false;
		CurItem = CurMenu + selected;
		CurItem->isSelect = true;

		CurMenu = CurItem->parentMenu;
		if (CurMenu == mainMenu)
			MenuWindow.title = defaultTitle;
		else
			MenuWindow.title = CurItem->title + 2;
		isChangeMenu = true;
	} else {
		exitMenu();
	}
}

/*顯示菜單*/
void DisplayMenuInit(MenuItem_Typedef* menu) {
	uint8_t topitem;
	uint8_t showItems;

	if (isChangeMenu == false)
		return;

	selected = getMenuSelectitem(menu);
	cur_sequence = selected;
	if (selected < menu->cursorPosition)
		menu->cursorPosition = 0;
	topitem = selected - menu->cursorPosition;
	if (menu->menuItemCount <= MenuWindow.itemsperpage)
		topitem = 0;

	//重繪menu介面
	MenuWindow.topitem = topitem;
	GUI_WindowsDraw(&MenuWindow);

	MenuScrollbar.topitem = topitem;
	MenuScrollbar.totalitems = menu->menuItemCount;
	GUI_ScrollbarDraw(&MenuScrollbar);

	showItems = MenuWindow.itemsperpage;
	if (menu->menuItemCount < MenuWindow.itemsperpage)
		showItems = menu->menuItemCount;
	for (int i = 0; i < showItems; i++) {
		MenuItem_Typedef* Item = &menu[topitem + i];
		GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1 + i * 15, Item);
	}

//	CurItem = menu + cur_sequence;
//	CurItem->isSelect = true;

	GUI_Refresh(); //刷新屏幕
	isChangeMenu = false;
}


void Menu_SetSelected(int item_index,bool selected){
	//判斷index是否越界
	CurItem = CurMenu + item_index;
	CurItem->isSelect = selected;
}

/*菜單運行*/
void Menu_Run(void) {
//	u8 keyState;
//	enum dir_e joystick2;
//	static int timeout;
	uint8_t showItems;
	Rotary_state rstate =state_no_changed;
	DisplayMenuInit(CurMenu);
	RotaryEcncorder_SetRange(0,CurMenu->menuItemCount-1);
	cur_rotateNum = RotaryEcncorder_GetCount();
	rstate = RotaryEcncorder_GetState();
	switch(rstate){
		case state_no_changed:
			break;
		case state_counter_clock_wise:
			//清除窗口內容
			GUI_RectangleFill(MenuWindow.x+1,MenuScrollbar.y,MenuScrollbar.x-1,MenuWindow.height-2,0);
			Menu_SetSelected(cur_sequence,false);
			//菜單項目序號--
			cur_sequence = cur_rotateNum;
			Menu_SetSelected(cur_sequence,true);
			//光標位置--
			CurMenu->cursorPosition--;
			if (CurMenu->menuItemCount <= MenuWindow.itemsperpage) {
				showItems = CurMenu->menuItemCount;
				if (CurMenu->cursorPosition < 0)
					CurMenu->cursorPosition = 0;
			} else {
				showItems = MenuWindow.itemsperpage;
				if (CurMenu->cursorPosition < 0) {
					CurMenu->cursorPosition = 0;
					if (MenuWindow.topitem > 0)
						MenuWindow.topitem--;
				}
			}
			for (int i = 0; i < showItems; i++) {
				MenuItem_Typedef* Item = CurMenu + MenuWindow.topitem + i;
				GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1 + i * 15, Item);
			}
			MenuScrollbar.topitem = cur_sequence;
			GUI_Scrollbar_SetPos(&MenuScrollbar);
			GUI_Refresh();
			break;
		case state_clock_wise:
			GUI_RectangleFill(MenuWindow.x+1,MenuScrollbar.y,MenuScrollbar.x-1,MenuWindow.height-2,0);
			Menu_SetSelected(cur_sequence,false);
			cur_sequence = cur_rotateNum;
			Menu_SetSelected(cur_sequence,true);
			//光標位置++
			CurMenu->cursorPosition++;
			if (CurMenu->menuItemCount <= MenuWindow.itemsperpage) {
				showItems = CurMenu->menuItemCount;
				if (CurMenu->cursorPosition >= CurMenu->menuItemCount)
					CurMenu->cursorPosition = CurMenu->menuItemCount - 1;
			} else {
				showItems = MenuWindow.itemsperpage;
				if (CurMenu->cursorPosition > MenuWindow.itemsperpage - 1) {
					CurMenu->cursorPosition = MenuWindow.itemsperpage - 1;
					if (MenuWindow.topitem < CurMenu->menuItemCount - MenuWindow.itemsperpage)
						MenuWindow.topitem++;
				}
			}
			for (int i = 0; i < showItems; i++) {
				MenuItem_Typedef* Item = CurMenu + MenuWindow.topitem + i;
				GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1 + i * 15, Item);
			}
			MenuScrollbar.topitem = cur_sequence;
			GUI_Scrollbar_SetPos(&MenuScrollbar);
			GUI_Refresh();
		break;
	}

//	switch(joystick2)
//	{
//
//		case FORWARD:	//PITCH向前

//			break;
//
//		case RIGHT:  //ROLL向右
//			gotoNextMenu();//進入下一級菜單
//			break;
//
//		case LEFT:  //ROLL向左
//			gotoLastMenu();//進入上一級菜單
//			break;
//
//		default :break;
//	}
//
//	keyState = getKeyState();
//	/*按下搖桿鍵執行菜單對應的動作*/
//	if(keyState == KEY_J2_SHORT_PRESS)
//	{
//		if(CurItem->Function != NULL)
//		{
//			CurItem->Function();
//		}
//	}
//	/*超時退出菜單*/
//	if(timeout++ > 100)
//	{
//		timeout = 0;
//		if(CurItem->parentMenu != NULL)
//		{
//			//恢復默認選中
//			CurItem->isSelect = false;
//			CurItem = CurMenu + selected;
//			CurItem->isSelect = true;
//		}
//		exitMenu();
//	}
}

