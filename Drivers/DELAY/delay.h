#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"
#include "config.h"

#if SUPPORT_OS ==1
	#include "FreeRTOS.h"
	#include "task.h"
#endif

void DWT_Delay_Init(void);
void DWT_Delay(uint32_t us);
void DWT_Delayms(uint32_t ms);
void delay_ms(uint32_t ms);
#define delay_us(x) DWT_Delay(x)










#endif
