#include <string.h>
#include "text.h"	
#include "SSD1322.h"
#include "font.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板V3
//漢字顯示驅動代碼
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//創建日期:2015/1/20
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
#define  LCD_WIDTH		256
#define  LCD_HIGH		64

//在指定位置開始顯示一個字符串
//支持自動換行
//(x,y):起始坐標
//width,height:區域
//str  :字符串
//f_w:字寬
//f_h:字高
//mode:0,非疊加方式;1,疊加方式
void show_str(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode)
{					
	uint16_t x0 = x;
	uint16_t y0 = y;
	while (*str != 0)
	{//數據未結束
		if (x > (x0 + LCD_WIDTH - f_h / 2))     //換行
		{
			y += f_h;
			x = x0;
		}
		if (y > (y0 + LCD_HIGH - f_h))
			break;     //越界返回
		if (*str == 13) //換行符號
		{
			y += f_h;
			x = x0;
			str++;
		}
		else
			OLED_ShowChar(x, y, *str, f_w / 2, f_h, mode);     //有效部分寫入
		str++;
		x += f_h / 2; //字符,為全字的一半

	}
}  			 		 
//在指定寬度的中間顯示字符串(置中顯示)
//如果字符長度超過了len,則用Show_Str顯示
//len:指定要顯示的寬度
void show_str_mid(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode,uint16_t len)
{
	uint16_t strlenth=0;
   	strlenth=strlen((char*)str);
	strlenth*=f_h/2;
	if(strlenth>len)show_str(x,y,str,f_w,f_h,mode);
	else
	{
		strlenth=(len-strlenth)/2;
	    show_str(strlenth+x,y,str,f_w,f_h,mode);
	}
}   

