/****************************************************************************************
* 文件名：WINDOW.C
* 功能：視窗管理。將進行各種視窗的繪製及視窗內部控制。
* 作者：黃紹斌
* 日期：2004.02.26
* 備註：
****************************************************************************************/
/****************************************************************************************
* 更新
* 功能：更新測試函數 void test_window(void)的功能展示
* 作者：詹育華
* 日期：2018.08.20
* 備註：
****************************************************************************************/


#include "windows.h"
#include "gui_basic.h"
#include "string.h"
#include "text.h"

#if  GUI_WINDOW_EN==1

//得到字符串的長度
uint16_t getWindowStringlen(const unsigned char *str)
{
	uint16_t strlenth=0;
	strlenth=strlen((const char*)str);
	strlenth*=6;

	return strlenth;
}

/****************************************************************************
* 名稱：GUI_WindowsDraw()
* 功能：顯示視窗。根據提供的視窗參數進行描繪視窗。
* 入口參數：win		窗口句柄
* 出口參數：返回0表示操作失敗，返回1表示操作成功
****************************************************************************/
uint8_t  GUI_WindowsDraw(WINDOWS *win)
{  
	/* 參數過濾，若窗口起出範圍，則返回0 */
	if( ( (win->width)<20 ) || ( (win->height)<20 ) ) return(0);	// 寬度、高度檢查，限制最小窗口
	if( (win->x + win->width ) > GUI_LCM_XMAX ) return(0);			// 窗口寬度是否溢出
	if( (win->y + win->height ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出

	/* 開始畫窗口 */
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, 0);// 清除視窗(全螢幕清除)
	GUI_Rectangle(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, 1);	// 畫窗口邊框
	GUI_HLine(win->x, win->y + 14, win->x + win->width - 1, disp_color);					// 畫標題目欄下邊框

	//填充標題欄
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + 14-1, 1);
	if(getWindowStringlen(win->title)>(win->width-2))
	{  
		return 0;//顯示的標題超過了窗體的寬度
	}	
	show_str_mid(win->x, win->y+1, win->title,12,12,0,win->width);//顯示標題
	return(1);
}


/****************************************************************************
* 名稱：GUI_WindowsDelete()
* 功能：刪除或隱藏視窗(標題含內容刪除)。
* 入口參數：win		窗口句柄
* 出口參數：返回0表示操作失敗，返回1表示操作成功
****************************************************************************/
uint8_t GUI_WindowsDelete(WINDOWS *win)
{  
	/* 參數過濾，若窗口起出範圍，則返回0 */
	if( ( (win->width)<20 ) || ( (win->height)<20 ) ) return(0);	// 寬度、高度檢查，限制最小窗口
	if( (win->x + win->width ) > GUI_LCM_XMAX ) return(0);			// 窗口寬度是否溢出
	if( (win->y + win->height ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出

	/* 消隱窗口 */
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, back_color);
	return(1);
}



/****************************************************************************
* 名稱：GUI_WindowsClr()
* 功能：清除視窗內容不含標題。
* 入口參數：win		窗口句柄
* 出口參數：無
* 說明：使用此函數前要先使用GUI_WindowsDraw()將窗口畫出。
****************************************************************************/
void GUI_WindowsClr(WINDOWS *win)
{ 
	uint8_t  x0, y0;
	uint8_t  x1, y1;

	/* 設置要清屏的區域 */
	x0 = win->x + 1;
	x1 = win->x + win->width - 2;
	y0 = win->y + 14;
	y1 = win->y + win->height - 2;

	/* 使用填充矩形實現清屏 */
	GUI_RectangleFill(x0, y0, x1, y1, back_color);
}


WINDOWS winxp=
{
.x = 0,
.y = 0,	
.width = 256,
.height = 64,
.itemsperpage = 3,
.topitem = 0,
.title = "XBOS",
};

//測試窗體
void test_window(void)
{
	GUI_WindowsDraw(&winxp);
	show_str_mid(winxp.x, winxp.y+15, "TEST123",12,12,1,winxp.width);
	show_str_mid(winxp.x, winxp.y+25, "TEST456",12,12,1,winxp.width);
	show_str_mid(winxp.x, winxp.y+37, "TEST789",12,12,1,winxp.width);
	GUI_Refresh();
	HAL_Delay(1000);
	GUI_WindowsClr(&winxp);
	GUI_Refresh();
	HAL_Delay(1000);
	GUI_WindowsDelete(&winxp);
	GUI_Refresh();
}


#endif












