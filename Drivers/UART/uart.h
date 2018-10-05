#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "task.h"
extern UART_HandleTypeDef huart1;
extern uint8_t aRxBuffer;

#define MAX_RECEIEVE_SIZE 256


extern TaskHandle_t uartTaskHandler;

#define UART_TASK_PRIORITY   				4  	 // Task Priority
#define UART_TASK_STACK_SIZE 				128	// Task Stack Size

void uart_task(void *pvParameters);
void USART1_IRQHandler(void);
uint8_t UART_Avaliable();
void uart_putchar(int ch);
void myprintf(char*format, ...);




#endif
