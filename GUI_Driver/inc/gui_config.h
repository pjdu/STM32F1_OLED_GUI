/****************************************************************************************
* 文件名：GUI_CONFIG.H
* 功能：用於裁剪GUI代碼。
* 作者：黃紹斌
* 日期：2004.03.11
* 備註：將宏設置為1時使能相應功能函數，為0或其它值時禁止。
****************************************************************************************/
#ifndef  GUI_CONFIG_H
#define  GUI_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "SSD1322.h"

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef NULL
	#define NULL 0
#endif


extern uint8_t const  DCB2HEX_TAB[8];


/* 定義顏色數據類型(可以是數據結構) */
#define  TCOLOR	  uint8_t 					//自行修改，原來是	uint8_t_t

/* 定義LCM像素數宏 */
#define  GUI_LCM_XMAX	OLED_PIXEL_WIDTH     /* 定義液晶x軸的像素數 */
#define  GUI_LCM_YMAX	OLED_PIXEL_HEIGHT	/* 定義液晶y軸的像素數 */

/* 定義前景色及背景色變量，用於ASCII碼、漢字、窗口、單色位圖顯示 */
extern TCOLOR	disp_color;
extern TCOLOR	back_color;

/****************************************************************************
* 名稱：GUI_CmpColor()
* 功能：判斷顏色值是否一致。
* 入口參數：color1		顏色值1
*		   color2		顏色值2
* 出口參數：返回1表示相同，返回0表示不相同。
* 說明：由於顏色類型TCOLOR可以是結構類型，所以需要用戶編寫比較函數。
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )

/****************************************************************************
* 名稱：GUI_CopyColor()
* 功能：顏色值複製。
* 入口參數：color1		目標顏色變量
*		   color2		源顏色變量
* 出口參數：無
* 說明：由於顏色類型TCOLOR可以是結構類型，所以需要用戶編寫複製函數。
****************************************************************************/
#define  GUI_CopyColor(color1, color2) 	*color1 = color2



/* 有寬度的直線函數 */
#define   GUI_LineWith_EN		0
/* 畫圓函數 */
#define   GUI_CircleX_EN		1
/* 橢圓函數 */
#define   GUI_EllipseX_EN		0
/* 填充函數 */
#define   GUI_FloodFill_EN		1
/* 向上及向下節點個數(填充函數) */
#define   DOWNP_N				20
#define   UPP_N					20
/* 圓弧函數 */
#define   GUI_ArcX_EN			1
/* 扇形函數 */
#define   GUI_Pieslice_EN		0



/* 窗口管理 */
#define   GUI_WINDOW_EN			1

/* 圖標菜單 */
#define  GUI_MenuIco_EN			0	

/* 下拉菜單 */
#define  GUI_MenuDown_EN		0


/* 漢字顯示函數 */
#define  GUI_PutHZ_EN			0


/* 單色圖形顯示函數 */
#define	 GUI_LoadPic_EN			0

/* 顏色轉換 */
#define  CONVERTCOLOR_EN		0



#endif
