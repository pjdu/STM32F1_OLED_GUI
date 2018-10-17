#ifndef __DECODE_COMMAND_H
#define __DECODE_COMMAND_H

#include "FreeRTOS.h"
#include "task.h"
#include "air_data.h"
#include "get_command.h"
#include "string.h"
#include "decode_command.h"
#include "uart.h"
#include "config.h"



extern TaskHandle_t decodeTaskHandler;

#define DECODE_TASK_PRIORITY   	CONFIG_DECODE_TASK_PRIORITY  	 // Task Priority
#define DECODE_TASK_STACK_SIZE 	CONFIG_DECODE_TASK_STACK_SIZE	// Task Stack Size

void decode_command_task();

#endif
