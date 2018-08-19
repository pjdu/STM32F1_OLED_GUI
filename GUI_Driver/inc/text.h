#ifndef __TEXT_H
#define __TEXT_H 
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板V3
//漢字顯示 驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//創建日期:2015/1/20
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

/*混合顯示漢字和字符*/
void show_str(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode);
/*在中間顯示混合字符*/
void show_str_mid(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode,uint16_t len);

#endif
