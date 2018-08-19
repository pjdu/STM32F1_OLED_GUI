/****************************************************************************************
* 文件名：MENU.H
* 功能：菜單顯示及操作，頭文件。
* 作者：黃紹斌
* 日期：2004.02.26
* 備註：
****************************************************************************************/
#ifndef  MENU_H
#define  MENU_H
#include <stdbool.h>
#include "gui_config.h"

//定義圖標菜單32X32
typedef struct MenuItem
{  	
	int8_t cursorPosition;			//菜單顯示時光標位置
	uint8_t menuItemCount;			//菜單包含項目數
	bool isSelect;					//顯示菜單的狀態 是否選中
	const uint8_t *icoSelected;		//菜單項目選中的ICO數據地址
	const uint8_t *icoUnselected;	//菜單項目沒選中的ICO數據地址
	const uint8_t *title;			//菜單項目內容
	void (*Function)(void); 		//菜單對應的動作
	struct MenuItem* parentMenu;	//上一級菜單
	struct MenuItem* childrenMenu;	//下一級菜單
}MenuItem_Typedef;


uint8_t  GUI_MenuIcoDraw(MenuItem_Typedef *ico);
void GUI_MenuItem_Selected(uint32_t x, uint32_t y, MenuItem_Typedef *item, bool isSelect);
uint8_t  GUI_MenuItemDraw(uint32_t x, uint32_t y, MenuItem_Typedef *item);


#endif



