#include "SSD1322.h"
#include "font.h"
extern SPI_HandleTypeDef hspi2;

#include <stm32f1xx_hal.h>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define	Shift		0x1C
#define Max_Column	0x3F			// 256/4-1
#define Max_Row		0x3F			// 64-1
#define	Brightness	0x0F

uint16_t gram[64][64] = { 0 }; // row : 64 col: 64 *4 = 256   u16 = [p1,p2,p3,p4]
static void write(uint8_t bytes, uint8_t is_data) {
	uint8_t tmp = bytes;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, RESET);
	if (is_data) {
		HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, SET);
	}
	/* We are sending commands */
	else {
		HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, RESET);
	}
	HAL_SPI_Transmit(&hspi2, &tmp, 1, 1000);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, SET);
}

static void Set_Column_Address(unsigned char a, unsigned char b) {
	Write_Command(0x15);			// Set Column Address
	Write_Data(a);				//   Default => 0x00
	Write_Data(b);				//   Default => 0x77
}

static void Set_Row_Address(unsigned char a, unsigned char b) {
	Write_Command(0x75);			// Set Row Address
	Write_Data(a);				//   Default => 0x00
	Write_Data(b);				//   Default => 0x7F
}

static void Set_Write_RAM() {
	Write_Command(0x5C);			// Enable MCU to Write into RAM
}

/*
 void Set_Read_RAM()
 {
 Write_Command(0x5D);			// Enable MCU to Read from RAM
 }
 */

static void Set_Remap_Format(unsigned char d) {
	Write_Command(0xA0);			// Set Re-Map / Dual COM Line Mode
	Write_Data(d);				//   Default => 0x40
	//     Horizontal Address Increment
	//     Column Address 0 Mapped to SEG0
	//     Disable Nibble Remap
	//     Scan from COM0 to COM[N-1]
	//     Disable COM Split Odd Even
	Write_Data(0x11);			//   Default => 0x01 (Disable Dual COM Mode)
}

static void Set_Start_Line(unsigned char d) {
	Write_Command(0xA1);			// Set Vertical Scroll by RAM
	Write_Data(d);				//   Default => 0x00
}

static void Set_Display_Offset(unsigned char d) {
	Write_Command(0xA2);			// Set Vertical Scroll by Row
	Write_Data(d);				//   Default => 0x00
}

static void Set_Display_Mode(unsigned char d) {
	Write_Command(0xA4 | d);			// Set Display Mode
	//   Default => 0xA4
	//     0xA4 (0x00) => Entire Display Off, All Pixels Turn Off
	//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
	//     0xA6 (0x02) => Normal Display
	//     0xA7 (0x03) => Inverse Display
}

static void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c) {
	Write_Command(0xA8 | a);
	// Default => 0x8F
	//   Select Internal Booster at Display On
	if (a == 0x00) {
		Write_Data(b);
		Write_Data(c);
	}
}

static void Set_Function_Selection(unsigned char d) {
	Write_Command(0xAB);			// Function Selection
	Write_Data(d);				//   Default => 0x01
	//     Enable Internal VDD Regulator
}

static void Set_Display_On_Off(unsigned char d) {
	Write_Command(0xAE | d);			// Set Display On/Off
	//   Default => 0xAE
	//     0xAE (0x00) => Display Off (Sleep Mode On)
	//     0xAF (0x01) => Display On (Sleep Mode Off)
}

static void Set_Phase_Length(unsigned char d) {
	Write_Command(0xB1);// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	Write_Data(d);//   Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 5~31 Display Clocks
	//     D[7:4] => Phase 2 Period in 3~15 Display Clocks
}

static void Set_Display_Clock(unsigned char d) {
	Write_Command(0xB3);	// Set Display Clock Divider / Oscillator Frequency
	Write_Data(d);				//   Default => 0xD0
	//     A[3:0] => Display Clock Divider
	//     A[7:4] => Oscillator Frequency
}

static void Set_Display_Enhancement_A(unsigned char a, unsigned char b) {
	Write_Command(0xB4);			// Display Enhancement
	Write_Data(0xA0 | a);			//   Default => 0xA2
	//     0xA0 (0x00) => Enable External VSL
	//     0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	Write_Data(0x05 | b);			//   Default => 0xB5
	//     0xB5 (0xB0) => Normal
	//     0xFD (0xF8) => Enhance Low Gray Scale Display Quality
}

static void Set_GPIO(unsigned char d) {
	Write_Command(0xB5);			// General Purpose IO
	Write_Data(d);			//   Default => 0x0A (GPIO Pins output Low Level.)
}

static void Set_Precharge_Period(unsigned char d) {
	Write_Command(0xB6);			// Set Second Pre-Charge Period
	Write_Data(d);				//   Default => 0x08 (8 Display Clocks)
}

static void Set_Precharge_Voltage(unsigned char d) {
	Write_Command(0xBB);			// Set Pre-Charge Voltage Level
	Write_Data(d);				//   Default => 0x17 (0.50*VCC)
}

static void Set_VCOMH(unsigned char d) {
	Write_Command(0xBE);			// Set COM Deselect Voltage Level
	Write_Data(d);				//   Default => 0x04 (0.80*VCC)
}

static void Set_Contrast_Current(unsigned char d) {
	Write_Command(0xC1);			// Set Contrast Current
	Write_Data(d);				//   Default => 0x7F
}

static void Set_Master_Current(unsigned char d) {
	Write_Command(0xC7);			// Master Contrast Current Control
	Write_Data(d);				//   Default => 0x0f (Maximum)
}

static void Set_Multiplex_Ratio(unsigned char d) {
	Write_Command(0xCA);			// Set Multiplex Ratio
	Write_Data(d);				//   Default => 0x7F (1/128 Duty)
}

static void Set_Display_Enhancement_B(unsigned char d) {
	Write_Command(0xD1);			// Display Enhancement
	Write_Data(0x82 | d);			//   Default => 0xA2
	//     0x82 (0x00) => Reserved
	//     0xA2 (0x20) => Normal
	Write_Data(0x20);
}

static void Set_Command_Lock(unsigned char d) {
	Write_Command(0xFD);			// Set Command Lock
	Write_Data(0x12 | d);			//   Default => 0x12
	//     0x12 => Driver IC interface is unlocked from entering command.
	//     0x16 => All Commands are locked except 0xFD.
}

static void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table
	Write_Data(0x0C);			//   Gray Scale Level 1
	Write_Data(0x18);			//   Gray Scale Level 2
	Write_Data(0x24);			//   Gray Scale Level 3
	Write_Data(0x30);			//   Gray Scale Level 4
	Write_Data(0x3C);			//   Gray Scale Level 5
	Write_Data(0x48);			//   Gray Scale Level 6
	Write_Data(0x54);			//   Gray Scale Level 7
	Write_Data(0x60);			//   Gray Scale Level 8
	Write_Data(0x6C);			//   Gray Scale Level 9
	Write_Data(0x78);			//   Gray Scale Level 10
	Write_Data(0x84);			//   Gray Scale Level 11
	Write_Data(0x90);			//   Gray Scale Level 12
	Write_Data(0x9C);			//   Gray Scale Level 13
	Write_Data(0xA8);			//   Gray Scale Level 14
	Write_Data(0xB4);			//   Gray Scale Level 15

	Write_Command(0x00);			// Enable Gray Scale Table
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//   區塊資料填充
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//	  Data: 0x00(滅) 0xff(亮)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_FillBlock(unsigned char a, unsigned char b,unsigned char c, unsigned char d,unsigned char Data){
	for(int y=c;y<d;y++)
	{
		for(int x=a;x<b;x++)
		{
			OLED_DrawPoint(x,y,Data);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_FillRam(unsigned char Data) {
	OLED_FillBlock(0x00, Max_Column, 0x00, Max_Row,Data);
}

void OLED_Clear()
{
	OLED_FillRam(0x00);
}
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode) {

	if (x > 255 || y > 63)
		return;
	uint16_t point = 0x000f;
	uint8_t group = x >> 2;
	uint8_t group_bit = 3 - (x % 4);
	point = point << (group_bit * 4);

	if (!mode) {
		point = ~point;
		gram[y][group] &= point;
	} else {
		gram[y][group] |= point;
	}

}
uint8_t OLED_ReadPoint(uint8_t x, uint8_t y) {

	if (x > 255 || y > 63)
		return 0;
	uint16_t point = 0x000f;
	uint8_t group = x >> 2;
	uint8_t group_bit = 3 - (x % 4);

	point = (gram[y][group] & (point << (group_bit * 4))) >> (group_bit * 4);
	point = point & 0x00ff;
	return point;

}
void OLED_RefreshGram() {
	int a = 0x00, b = 0x3F; // column start ,column end
	int c = 0x00, d = 0x3F; // row start ,row end
	Set_Column_Address(Shift + a, Shift + b);
	Set_Row_Address(c, d);
	Set_Write_RAM();
	for (int y = 0; y < (d - c+1); y++) {
		for (int x = 0; x < (b - a+1); x++) {
			Write_Data((gram[y][x] & 0xff00) >> 8); //draw 3,4 point
			Write_Data(gram[y][x] & 0x00ff); 		//draw 1,2 point
		}
	}
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t f_w, uint8_t f_h,uint8_t mode){
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(f_h/8+((f_h%8)?1:0))*f_w;//得到自由分辨字符所占的字节数
	chr=chr-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{
#if Font_06_08_EN==1
		if(f_w==6&&f_h==8)temp=asc2_0608[chr][t];		//调用0608ascii字体
#endif
#if Font_06_12_EN==1
		else if(f_w==6&&f_h==12)temp=asc2_0612[chr][t];	//调用0612ascii字体
#endif
#if Font_12_24_EN==1
		else if(f_w==12&&f_h==24)temp=asc2_1224[chr][t];//调用1224ascii字体
#endif
#if Font_06_08_EN==1 || Font_06_12_EN==1 || Font_12_24_EN==1
		else return;	//没有的字库
#endif
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==f_h)
			{
				y=y0;
				x++;
				break;
			}
		}
	}
}

//显示图片
//x,y:起点坐标
//p_w:图片宽（单位像素）
//p_h:图片高（单位像素）
//*p:图片起始地址
void OLED_ShowPicture(uint8_t x,uint8_t y,const uint8_t *p,uint8_t p_w,uint8_t p_h)
{
	uint8_t temp,i,col,row;
	uint8_t y0=y;
	uint8_t width=p_w;
	if(x+p_w>128)width=128-p_w;//实际显示宽度
	uint8_t high=p_h;
	if(y+p_h>64)high=64-p_h;//实际显示高度
	uint8_t exp_col_bytes=(p_h/8+((p_h%8)?1:0));//显示一列的字节数
	uint8_t act_col_bytes=(high/8+((high%8)?1:0));//实际显示一列的字节数

	for(row=0;row<width;row++)//列++
	{
		for(col=0;col<act_col_bytes;col++)//显示一列
		{
			temp = p[col+row*exp_col_bytes];
			for(i=0;i<8;i++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,1);
				else OLED_DrawPoint(x,y,0);
				temp<<=1;
				y++;
				if((y-y0)==high)
				{
					y=y0;
					x++;
					break;
				}
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init() {
	uint8_t dummy = 0x0;

	//LCD_RST_SetLow();

//	for(i=0;i<200;i++)
//	{
//		HAL_Delay(1);
//	}

	//LCD_RST_SetHigh();
	HAL_SPI_Transmit(&hspi2, &dummy, 1, 1000);

	Set_Command_Lock(0x12);			// Unlock Basic Commands (0x12/0x16)
	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 80 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);	// Set Mapping RAM Display Start Line (0x00~0x7F)
	Set_Remap_Format(0x14);			// Set Horizontal Address Increment
	//     Column Address 0 Mapped to SEG0
	//     Disable Nibble Remap
	//     Scan from COM[N-1] to COM0
	//     Disable COM Split Odd Even
	//     Enable Dual COM Line Mode
	Set_GPIO(0x00);				// Disable GPIO Pins Input
	Set_Function_Selection(0x01);		// Enable Internal VDD Regulator
	Set_Display_Enhancement_A(0xA0, 0xFD);	// Enable External VSL
	// Set Low Gray Scale Enhancement
	Set_Contrast_Current(0x9F);		// Set Segment Output Current
	Set_Master_Current(Brightness);	// Set Scale Factor of Segment Output Current Control
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Phase_Length(0xE2);	// Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	Set_Display_Enhancement_B(0x20);// Enhance Driving Scheme Capability (0x00/0x20)
	Set_Precharge_Voltage(0x1F);	// Set Pre-Charge Voltage Level as 0.60*VCC
	Set_Precharge_Period(0x08);		// Set Second Pre-Charge Period as 8 Clocks
	Set_VCOMH(0x07);	// Set Common Pins Deselect Voltage Level as 0.86*VCC
	Set_Display_Mode(0x02);			// Normal Display Mode (0x00/0x01/0x02/0x03)
	Set_Partial_Display(0x01, 0x00, 0x00);	// Disable Partial Display

	OLED_Clear();				// Clear Screen

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)

}
