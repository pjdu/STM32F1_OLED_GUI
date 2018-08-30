#include "iwdgtask.h"




TaskHandle_t iwdgTaskHandler;

//iwdg 獨立看門狗任務，設定時間2秒內需重設一次看門狗計數，否則重啟
void iwdg_Task(void)
{
	HAL_IWDG_Init(&hiwdg);
	while(1){
		HAL_IWDG_Refresh(&hiwdg);
		vTaskDelay(1800/portTICK_PERIOD_MS);
	}
}
