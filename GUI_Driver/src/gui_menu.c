#include "gui_menu.h"
#include "gui_basic.h"
#include "text.h"
#include "SSD1322.h"

/****************************************************************************
* 名稱：GUI_MenuItemDraw()
* 功能：顯示條目菜單
* 入口參數：ico		圖標菜單句柄
* 出口參數：返回0表示操作失敗，返回1表示操作成功
****************************************************************************/
uint8_t  GUI_MenuItemDraw(uint32_t x, uint32_t y, MenuItem_Typedef *item)
{  
	if( x>GUI_LCM_XMAX || y>GUI_LCM_YMAX) return(0);
	if(y<2) return(0);
	if(x<18) return(0);
	if( (item->title)==NULL )  return(0);

	if(item->isSelect == false)
	{  
		if(item->icoUnselected != NULL)
			OLED_ShowPicture(x-18, y-2, (uint8_t *)item->icoUnselected ,16, 16);
		show_str(x, y,(uint8_t *) item->title,12,12,1);
	}
	else
	{  
		if(item->icoSelected != NULL)
			OLED_ShowPicture(x-18, y-2, (uint8_t *)item->icoSelected ,16, 16);
		show_str(x,y,(uint8_t *) item->title, 12,12,0);
	}
	return(1);
}


