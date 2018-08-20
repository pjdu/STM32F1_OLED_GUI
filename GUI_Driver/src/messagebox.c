#include "messagebox.h"
#include "gui_basic.h"
#include "string.h"
#include "button.h"
#include "text.h"

//得到字符串的長度
uint16_t getMessageBoxStringlen(uint8_t *str)
{
	uint16_t strlenth=0;
	strlenth=strlen((const char*)str);//字節數
	strlenth*=6;//一個字節水平6個點(12x12漢字)
	return strlenth;
}

Button_Typedef btn_ok_messagebox={
20,
20,
48,
15,
"OK",
1
};

Button_Typedef btn_cancel_messagebox={
20,
20,
48,
15,
"cancel",
1
};


//畫消息框
uint8_t GUI_MessageBoxDraw(MessageBox_Typedef* messagebox)
{
	/* 參數過濾，若窗口起出範圍，則返回0 */
	if( ( (messagebox->with)<20 ) || ( (messagebox->hight)<20 ) ) return(0);// 寬度、高度檢查，限制最小窗口
	if( (messagebox->x + messagebox->with ) > GUI_LCM_XMAX ) return(0);		// 窗口寬度是否溢出
	if( (messagebox->y + messagebox->hight ) > GUI_LCM_YMAX ) return(0);	// 窗口高度是否溢出

	/* 開始畫窗口 */
	GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, 0);//清屏
	GUI_Rectangle(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, 1);// 畫窗口(邊框)
	GUI_HLine(messagebox->x, messagebox->y + 14, messagebox->x + messagebox->with - 1, disp_color);		// 畫標題下邊框

	//填充標題欄
	GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + 14-1, 1);
	if(getMessageBoxStringlen(messagebox->title)>(messagebox->with-2))
	{  	
		return 0;//顯示的標題超過了窗體的寬度
	} 
	//顯示標題
	show_str_mid(messagebox->x, messagebox->y+1,messagebox->title,12,12,0,messagebox->with);


	//確定按鈕
	btn_ok_messagebox.state = 0;
	btn_ok_messagebox.title = messagebox->ok;
	btn_ok_messagebox.x = messagebox->x+5;
	btn_ok_messagebox.y = messagebox->y+messagebox->hight-1 -btn_ok_messagebox.height -3;
	GUI_DrawButton(&btn_ok_messagebox);

	//取消按鈕
	btn_cancel_messagebox.state = 1;
	btn_cancel_messagebox.title = messagebox->cancel;
	btn_cancel_messagebox.x = messagebox->x+ messagebox->with -btn_cancel_messagebox.width-1-5;
	btn_cancel_messagebox.y = messagebox->y+messagebox->hight-1 -btn_cancel_messagebox.height-3;	 
	GUI_DrawButton(&btn_cancel_messagebox); 

	//顯示消息
	show_str(messagebox->x+6, messagebox->y+1+16,messagebox->msg, 12,12,1);
	return(1);
}

//設置按鈕狀態
void GUI_MessageBoxButtonStatus(MessageBox_Typedef* messagebox, uint8_t status)
{	
	uint8_t ok_status;
	uint8_t cancel_status;

	if(status)
	{
		ok_status =1;
		cancel_status  =0;			
	}else
	{
		ok_status =0;
		cancel_status  =1;	
	}
	//確定按鈕
	GUI_Button_Selected(&btn_ok_messagebox,ok_status);

	//取消按鈕
	GUI_Button_Selected(&btn_cancel_messagebox,cancel_status);

}

//得到當前選擇結果
uint8_t GUI_MessageBoxResult(MessageBox_Typedef* messagebox)
{
	return btn_ok_messagebox.state;	
}

//刪除消息框
uint8_t GUI_MessageBoxDelete(MessageBox_Typedef* messagebox)
{	
   if( ( (messagebox->with)<20 ) || ( (messagebox->hight)<20 ) ) return(0);		// 寬度、高度檢查，限制最小窗口
   if( (messagebox->x + messagebox->with ) > GUI_LCM_XMAX ) return(0);			// 窗口寬度是否溢出
   if( (messagebox->y + messagebox->hight ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出
    
   /* 消隱窗口 */
   GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, back_color);
   return(1);
}

MessageBox_Typedef MessageBox={
	0,
	0,
	256,
	64,
	"reset",
	"system will be resetni hao a1234?",
	"OK",
	"Cancel"
};


//測試消息框
void test_MessageBox(void)
{
 GUI_MessageBoxDraw(&MessageBox);
 GUI_Refresh();
 HAL_Delay(2000);
 GUI_MessageBoxButtonStatus(&MessageBox, 1);
 GUI_Refresh();
 HAL_Delay(2000);
 GUI_MessageBoxButtonStatus(&MessageBox, 0);
 GUI_Refresh();
 HAL_Delay(2000);
 GUI_MessageBoxDelete(&MessageBox);


}




