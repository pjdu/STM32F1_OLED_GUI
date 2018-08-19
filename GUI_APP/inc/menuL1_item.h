#ifndef __MENUL1_ITEM_H
#define __MENUL1_ITEM_H
//#include "sys.h"
#include "gui_menu.h"
//#include "remoter_ctrl.h"

/********************************************************************************	 
 * 本程序只供學習使用，未經作者許可，不得用於其它任何用途
 * ALIENTEK MiniFly_Remotor
 * 一級菜單驅動代碼
 * 正點原子@ALIENTEK
 * 技術論壇:www.openedv.com
 * 創建日期:2018/6/1
 * 版本：V1.0
 * 版權所有，盜版必究。
 * Copyright(C) 廣州市星翼電子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

extern MenuItem_Typedef  mainMenu[];
extern const uint8_t finger_img[32];

void mainMenuInit(void);
//void expModuleMenuSet(enum expModuleID id);

#endif /*__MENUL1_ITEM_H*/
