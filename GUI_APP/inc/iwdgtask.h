#ifndef __IWDGTASK_H
#define __IWDGTASK_H

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

extern IWDG_HandleTypeDef hiwdg;
extern TaskHandle_t iwdgTaskHandler;

#define IWDG_TASK_PRIORITY   				10  	 // Task Priority
#define IWDG_TASK_STACK_SIZE 				70		// Task Stack Size
void iwdg_Task(void);

#endif
