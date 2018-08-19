#ifndef  __SCROLLBAR_H
#define  __SCROLLBAR_H

#include "gui_config.h"

//定義一個滾動條類型
typedef  struct
{
 uint32_t x;             
 uint32_t y;
 uint32_t width;
 uint32_t height;

 uint32_t totalitems;  	  	//總條目數
 uint32_t itemsperpage;		//每頁顯示的條目數
 uint32_t topitem;			//最頂端的條目
 uint32_t scbbarlen;		//滾條長度	
	
}Scrollbar_Typedef;

void test_scrollbar(void);

void GUI_ScrollbarDelete(Scrollbar_Typedef *scbx);
void GUI_Scrollbar_SetPos(Scrollbar_Typedef *scbx);
void GUI_ScrollbarDraw(Scrollbar_Typedef *scbx);



#endif




