/****************************************************************************************
* 文件名：WINDOW.C
* 功能：窗口管理。將進行各種窗口的給制及窗口內部控制。
* 作者：黃紹斌
* 日期：2004.02.26
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
* 功能：顯示窗口。根據提供的窗口參數進行畫窗口。
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
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, 0);// 這尼瑪不是清屏麼（黑）
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
* 名稱：GUI_WindowsHide()
* 功能：消隱窗口。
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
* 功能：清屏窗口。
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
	GUI_Refresh();
}


#endif












