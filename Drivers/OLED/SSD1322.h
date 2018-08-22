/* 
 * File:   SSD1322.h
 * Author: WTwork
 *
 */

#include <stm32f1xx_hal.h>
#define OLED_PIXEL_WIDTH    256
#define OLED_PIXEL_HEIGHT	64

#define SSD1322_Max_Column	0x3F			// 256/4-1
#define SSD1322_Max_Row		0x3F			// 64-1

#ifndef SSD1322_H
#define	SSD1322_H

#ifdef	__cplusplus
extern "C" {
#endif

/*Private Function Access only on this file*/
//static void write(uint8_t bytes, uint8_t is_data);
#define Write_Command(x)  write(x, 0)
#define Write_Data(x)  write(x, 1)





/*Public Function*/
void OLED_Clear();
void OLED_FillRam(unsigned char Data);
void OLED_FillBlock(unsigned char a, unsigned char b,unsigned char c, unsigned char d,unsigned char Data);

void OLED_Init();
void OLED_DrawPoint(uint8_t x, uint8_t y,uint8_t mode);
uint8_t OLED_ReadPoint(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t f_w, uint8_t f_h,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t f_w,uint8_t f_h);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t f_w,uint8_t f_h);
void OLED_ShowPicture(uint8_t x,uint8_t y,const uint8_t *p,uint8_t p_w,uint8_t p_h);
void OLED_RefreshGram();

#ifdef	__cplusplus
}
#endif

#endif	/* SSD1322_H */

