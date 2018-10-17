#ifndef __IWDGTASK_H
#define __IWDGTASK_H

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

extern IWDG_HandleTypeDef hiwdg;
extern TaskHandle_t iwdgTaskHandler;

#define IWDG_TASK_PRIORITY   	CONFIG_IWDG_TASK_PRIORITY  	 // Task Priority
#define IWDG_TASK_STACK_SIZE 	CONFIG_IWDG_TASK_STACK_SIZE // Task Stack Size
void iwdg_Task(void);

#endif
