#include "iwdgtask.h"




TaskHandle_t iwdgTaskHandler;



static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Reload = 2500;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


//iwdg 獨立看門狗任務，設定時間2秒內需重設一次看門狗計數，否則重啟
void iwdg_Task(void)
{
	MX_IWDG_Init();
	while(1){
		HAL_IWDG_Refresh(&hiwdg);
		vTaskDelay(1800/portTICK_PERIOD_MS);
	}
}
