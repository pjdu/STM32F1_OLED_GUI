#include "uart.h"

uint8_t aRxBuffer;
static uint8_t RxBuffer_Head;
static uint8_t RxBuffer_Tail;
static uint8_t RxBuffer_Len;
static uint8_t Uart_RxRingBuffer[MAX_RECEIEVE_SIZE];
TaskHandle_t uartTaskHandler;

void UART_Receieve_Init(){
	RxBuffer_Head = 0;
	RxBuffer_Tail = 0;
	RxBuffer_Len = 0;
	HAL_UART_Receive_IT(&huart1,&aRxBuffer,1);
}


static void UART_RxBuffer_Write()
{
	if(RxBuffer_Tail > MAX_RECEIEVE_SIZE -1)
	{
		RxBuffer_Tail = 0;
	//	RxBuffer_Len  = MAX_RECEIEVE_SIZE - RxBuffer_Head + RxBuffer_Tail;
	}

	Uart_RxRingBuffer[RxBuffer_Tail++] = aRxBuffer;

}

static uint8_t UART_RxBuffer_isEmpty()
{
	return RxBuffer_Head == RxBuffer_Tail;
}
uint8_t UART_RxBuffer_read()
{
	while(UART_RxBuffer_isEmpty());
	if(RxBuffer_Head > MAX_RECEIEVE_SIZE -1) RxBuffer_Head =0;
	RxBuffer_Len--;
	return Uart_RxRingBuffer[RxBuffer_Head++];
}

uint8_t UART_Avaliable()
{
	return RxBuffer_Len;
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
  HAL_UART_Receive_IT(&huart1,&aRxBuffer,1);
}

// uart interupt handler callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	/*
	 * UART Handle impelmentation
	 * */
	if(RxBuffer_Len >= 255)
	{
		RxBuffer_Len=0;
	}
	RxBuffer_Len++;
	UART_RxBuffer_Write();

}


// uart process task
void uart_task(void *pvParameters)
{
	char *buf;
	UART_Receieve_Init();
	while(1)
	{
		if(RxBuffer_Len > 0)
		{
			buf = pvPortMalloc( RxBuffer_Len * sizeof(uint8_t));
			for(int i = 0 ; i<RxBuffer_Len;i++)
			{
				buf[i] = UART_RxBuffer_read();
				HAL_UART_Transmit(&huart1,&(buf[i]),1,1000);
			}
			vPortFree(buf);
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}
