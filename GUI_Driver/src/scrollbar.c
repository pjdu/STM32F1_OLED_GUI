#include "scrollbar.h"
#include "gui_basic.h"
//#include "delay.h"
#include "SSD1322.h"

#define SCROLL_BTN_WIDTH  10  //按鈕的寬度
#define SCROLL_BTN_HEIGHT 10	//按鈕的高度

//上按鈕
const unsigned char scroll_btn_up[8]={
0x02,0x0E,0x3E,0x7E,0x7E,0x3E,0x0E,0x02,
};

//下按鈕
const unsigned char scroll_btn_down[8]={
0x40,0x70,0x7C,0x7E,0x7E,0x7C,0x70,0x40,
};

//設置滾動條的位置
void GUI_Scrollbar_SetPos(Scrollbar_Typedef *scbx)
{	
	uint16_t scbpos;  //滾動條偏移
	uint16_t scblen;  //滾動條可以滾動的長度
	uint16_t x,y,xlen,ylen;
//	uint8_t scbplen;
//	
//	scbplen = SCROLL_BTN_HEIGHT;  //得到按鈕的高度
	
	scblen=scbx->height-2*SCROLL_BTN_HEIGHT-scbx->scbbarlen;//得到滾動條滾動空間的長度
	x=scbx->x+1;                  //滾動條起始坐標x
	y=scbx->y+SCROLL_BTN_HEIGHT;  //滾動條起始坐標y
	xlen=scbx->width-2-1;         //滾動條寬度 -1
	ylen=scblen+scbx->scbbarlen-1;//滾動條長度 -1
	
	//清除滾動條
	GUI_RectangleFill(x ,y,x+xlen,y+ylen, 0);
	if(scbx->totalitems <= scbx->itemsperpage)//總條目不夠一頁顯示的
	{
		scbpos=0; //偏移0
	}
	else //不止一頁
	{
		scbpos=(scbx->topitem*scblen)/(scbx->totalitems-1);//計算當前滾動條偏移的位置
	}
	
	//畫出滾動條
	GUI_RectangleFill(x ,y+scbpos,x+xlen,y+scbpos+scbx->scbbarlen, 1);
	//GUI_Refresh();	//刷新屏幕
}



//創建一個滾動條
void GUI_ScrollbarDraw(Scrollbar_Typedef *scbx)
{
	uint32_t scbactlen=0;//滾動條可以滾動的空間長度
	if(scbx==NULL) return;
	if(scbx->width<SCROLL_BTN_WIDTH) return;
	if(scbx->height<2*SCROLL_BTN_HEIGHT) return;
	
	//畫外邊框
	GUI_Rectangle(scbx->x, scbx->y, scbx->x+scbx->width-1, scbx->y+scbx->height-1, 1);
	//畫上下兩個按鈕
	OLED_ShowPicture(scbx->x+(scbx->width-8)/2, scbx->y+1, (uint8_t *)scroll_btn_up, 8, 8);
	OLED_ShowPicture(scbx->x+(scbx->width-8)/2, scbx->y+scbx->height-9, (uint8_t *)scroll_btn_down, 8, 8);
	//畫按鈕邊框
	GUI_Line(scbx->x, scbx->y+9, scbx->x+scbx->width-1, scbx->y+9,1);
	GUI_Line(scbx->x, scbx->y+scbx->height-10, scbx->x+scbx->width-1, scbx->y+scbx->height-10,1);
	
	
	//可供滾動條滾動的空間範圍
	scbactlen=scbx->height -2*SCROLL_BTN_HEIGHT;
	
	//每頁滾動條目大於總條目數 滾動條目等於最大可滾動的控件
	if(scbx->itemsperpage >= scbx->totalitems)scbx->scbbarlen=scbactlen;
	//每頁滾動條目小於總條目數
	else scbx->scbbarlen=(scbactlen*scbx->itemsperpage)/scbx->totalitems;//得到滾動條的長度
	//設置滾動條的位置
	GUI_Scrollbar_SetPos(scbx);
}


//刪除一個滾動條
void GUI_ScrollbarDelete(Scrollbar_Typedef *scbx)
{
  GUI_RectangleFill(scbx->x, scbx->y, scbx->x+scbx->width-1, scbx->y+scbx->height-1, 0);

}

Scrollbar_Typedef Scrollbar=
{
245,
13,
10,
50,
	
9,
3,
9,
0
};

//測試滾動條函數
void test_scrollbar(void){

	GUI_ScrollbarDraw(&Scrollbar);
	GUI_Refresh();								//刷新屏幕
	HAL_Delay(2000);
	Scrollbar.topitem = 1;
	GUI_Scrollbar_SetPos(&Scrollbar);
	GUI_Refresh();								//刷新屏幕
	HAL_Delay(2000);
	GUI_ScrollbarDraw(&Scrollbar);
	
}




