/*
 * rotary_encorder.h
 *
 *  Created on: 2018年8月21日
 *      Author: user
 */

#ifndef ROTARY_ENCODER_ROTARY_ENCORDER_H_
#define ROTARY_ENCODER_ROTARY_ENCORDER_H_
#include <stm32f1xx_hal.h>
#include <stdint.h>
extern TIM_HandleTypeDef htim2;

typedef enum {
	button_press = 0,
	button_debouce = 1,
	button_release = 2,
	button_long_press = 3
} Button_state;

typedef enum{
	state_no_changed,
	state_clock_wise,
	state_counter_clock_wise
}Rotary_state;

Button_state RotaryEcncorder_ButtonScan(void);
void RotaryEcncorder_Init();
int32_t RotaryEcncorder_GetCount();
Rotary_state RotaryEcncorder_GetState();
int32_t RotaryEcncorder_SetCount(int32_t count);
int32_t RotaryEcncorder_SetRange(int32_t min ,int32_t max);
#endif /* ROTARY_ENCODER_ROTARY_ENCORDER_H_ */
