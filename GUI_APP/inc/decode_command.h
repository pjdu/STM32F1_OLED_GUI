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



extern int device_number;
extern Device_Msg dev_array[MAX_DEVICE_SIZE];

extern TaskHandle_t decodeTaskHandler;

#define DECODE_TASK_PRIORITY   				5  	 // Task Priority
#define DECODE_TASK_STACK_SIZE 				128	// Task Stack Size
void decode_command_task();

#endif
