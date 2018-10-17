#include "delay.h"

/*
 * Simple microseconds delay routine, utilizingARM's DWT
 * (Data Watchpoint and Trace Unit) and HAL library.
 * Intended to use with gcc compiler, but can be easily edited
 * for any other C compiler.
 * Max K
 *
 *
 * This file is part of DWT_Delay package.
 * DWT_Delay is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * us_delay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 * http://www.gnu.org/licenses/.
 */

extern uint32_t SystemCoreClock;

void DWT_Delay_Init(void)
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

/**
 * Delay routine itself.
 * Time is in microseconds (1/1000000th of a second), not to be
 * confused with millisecond (1/1000th).
 *
 * @param uint32_t us  Number of microseconds to delay for
 */
void DWT_Delay(uint32_t us) // microseconds
{
	int32_t targetTick = DWT->CYCCNT + us * (SystemCoreClock/1000000);
	while (DWT->CYCCNT <= targetTick);
}


void DWT_Delayms(uint32_t ms)
{
	int32_t targetTick = DWT->CYCCNT + ms * (SystemCoreClock/1000);
	while (DWT->CYCCNT <= targetTick);
}

void delay_ms(uint32_t ms){
#if SUPPORT_OS == 1
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
		vTaskDelay(ms);
	}
	else{
		DWT_Delayms(ms);
	}
#else
	DWT_Delayms(ms);
#endif

}


