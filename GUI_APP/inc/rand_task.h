#ifndef _RAND_TASK_H_
#define _RAND_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t randTaskHandler;

#define RAND_TASK_PRIORITY   				4  	 // Task Priority
#define RAND_TASK_STACK_SIZE 				128		// Task Stack Size

void rand_task();

#endif
