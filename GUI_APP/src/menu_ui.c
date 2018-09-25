#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "rotary_encorder.h"
#include "menuL1_item.h"
#include "main_ui_page.h"
#include "config.h"
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

//golbal var
int32_t cur_rotateNum = 0;

static MenuItem_Typedef* CurMenu = mainMenu;  //當前Menu的控制元件
static MenuItem_Typedef* CurItem;

static int selected;
static int cur_sequence = 0; //某一級菜單選中項目的序號
static bool isChangeMenu = true; //menu狀態是否改變
const uint8_t* defaultTitle = "IAQ Weather";

//窗體
WINDOWS MenuWindow = { .x = 0, .y = 0, .width = 256, .height = 64,
		.itemsperpage = 3, .topitem = 0, .title = "IAQ Weather" };

//主窗體滾動條
Scrollbar_Typedef MenuScrollbar = { .x = 245, .y = 14, .width = 10,
		.height = 50, .itemsperpage = 3, .topitem = 0, .scbbarlen = 0, };

TaskHandle_t MenuTaskHandler;

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

void Menu_SetSelected(int item_index, bool selected) {
	//判斷index是否越界
	CurItem = CurMenu + item_index;
	CurItem->isSelect = selected;
}

/*菜單運行*/
void Menu_Task(void *pvParameters) {

	uint8_t showItems;
	Rotary_state rstate = state_no_changed;
	DisplayMenuInit(CurMenu);
	RotaryEcncorder_Init();
	RotaryEcncorder_SetRange(0, CurMenu->menuItemCount - 1);
	while (1) {

		//按鈕事件處理
		if (EventGroupHandler != NULL) {
			EventBits_t val;
			val = xEventGroupWaitBits(EventGroupHandler,
			BUTTON_PRESS_EVENT,
			pdTRUE,	 //等待到bit自動清除
			pdFALSE, //任一事件發生進入
			10 / portTICK_PERIOD_MS);

			switch(val)
			{
				case BUTTON_PRESS_EVENT:
					CurItem = CurMenu + cur_sequence;
					if (CurItem->Function != NULL) {
						CurItem->Function();
					}
					break;
				case NULL_EVENT_RAISE:
					break;
			}
		}
		// 旋轉編碼器旋轉界面處理
		cur_rotateNum = RotaryEcncorder_GetCount();
		rstate = RotaryEcncorder_GetState();
		switch (rstate) {
		case state_no_changed:
			break;
		case state_counter_clock_wise:
		case state_clock_wise:
			//清除窗口內容
			GUI_RectangleFill(MenuWindow.x + 1, MenuScrollbar.y,
					MenuScrollbar.x - 1, MenuWindow.height - 2, 0);
			Menu_SetSelected(cur_sequence, false);
			cur_sequence = cur_rotateNum;
			Menu_SetSelected(cur_sequence, true);
			CurMenu->cursorPosition = cur_sequence;
			if (CurMenu->menuItemCount <= MenuWindow.itemsperpage) {
				showItems = CurMenu->menuItemCount;
			} else {
				if (CurMenu->menuItemCount - cur_sequence< MenuWindow.itemsperpage) {
					showItems = CurMenu->menuItemCount - cur_sequence;
				} else {
					showItems = MenuWindow.itemsperpage;
				}
				MenuWindow.topitem = cur_sequence;
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
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

