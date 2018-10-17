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
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "config.h"
extern TIM_HandleTypeDef htim2;
extern EventGroupHandle_t EventGroupHandler;

typedef enum {
	button_normal = 0,
	button_press,
	button_debouce,
	button_release,
	button_long_press,
	button_long_press_1s
} Button_state;

typedef enum{
	state_no_changed,
	state_clock_wise,
	state_counter_clock_wise
}Rotary_state;

/*Button_task==========================================*/
#define BUTTON_TASK_PRIORITY   				CONFIG_BUTTON_TASK_PRIORITY  		// Task Priority
#define BUTTON_TASK_STACK_SIZE 				CONFIG_BUTTON_TASK_STACK_SIZE		// Task Stack Size
TaskHandle_t ButtonTaskHandler;						// Task Handler

void Button_task(void *pvParameters);
Button_state RotaryEcncorder_ButtonScan(void);
void RotaryEcncorder_Init();
int32_t RotaryEcncorder_GetCount();
Rotary_state RotaryEcncorder_GetState();
void RotaryEcncorder_SetCount(int32_t count);
void RotaryEcncorder_SetRange(int32_t min ,int32_t max);
#endif /* ROTARY_ENCODER_ROTARY_ENCORDER_H_ */
