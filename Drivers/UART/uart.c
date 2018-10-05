#include "uart.h"
#include "get_command.h"
#include "air_data.h"

// uart 相關變數
uint8_t aRxBuffer; 	   	// 接收中斷緩存
uint8_t RxBuffer_Head; 	// 環形隊列頭指標
uint8_t RxBuffer_Tail; 	// 環形隊列尾指標
volatile uint8_t RxBuffer_Len; // 環形隊列資料長度
uint8_t Uart_RxRingBuffer[MAX_RECEIEVE_SIZE]; //環形隊列

TaskHandle_t uartTaskHandler;


void UART_Receieve_Init() {
	RxBuffer_Head = 0;
	RxBuffer_Tail = 0;
	RxBuffer_Len = 0;
	HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
}

static uint8_t UART_RxBuffer_isEmpty() {
	return RxBuffer_Head == RxBuffer_Tail;
}
uint8_t UART_RxBuffer_read() {
	while (UART_RxBuffer_isEmpty())
		;
	if (RxBuffer_Head > MAX_RECEIEVE_SIZE - 1)
		RxBuffer_Head = 0;
	RxBuffer_Len--;
	return Uart_RxRingBuffer[RxBuffer_Head++];
}

uint8_t UART_Avaliable() {
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
void USART1_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart1);
	HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
}

// uart interupt handler callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/*
	 * UART Handle impelmentation
	 * */
	if (huart->Instance == USART1) {
		if (RxBuffer_Len > MAX_RECEIEVE_SIZE - 1) {
			RxBuffer_Len = 0;
		}

		if (RxBuffer_Tail > MAX_RECEIEVE_SIZE - 1) {
			RxBuffer_Tail = 0;
			//	RxBuffer_Len  = MAX_RECEIEVE_SIZE - RxBuffer_Head + RxBuffer_Tail;
		}
		Uart_RxRingBuffer[RxBuffer_Tail] = aRxBuffer;
		RxBuffer_Len++;
		RxBuffer_Tail++;
	}
}

void uart_putchar(int ch)
{
	HAL_UART_Transmit(&huart1,&ch,1,0xffff);
}

// vsprintf stack 基本需要200起跳
//功能:和printf的功能相同,但將printf導入到uart周邊進行輸出
////RTOS Support
////buffer 是藉由rtos,採用動態記憶體配置，若無rtos,可自行定義一個全域變數矩陣如:buffer[200];
//void myprintf(char*format, ...)
//{
//	int ret;
//	va_list aptr;
//	char* buffer;
//	buffer = pvPortMalloc(sizeof(char)*strlen(format));
//
//	va_start(aptr,format);
//	//ret = vsnprintf(buffer,sizeof(char)*strlen(format),format,aptr);
//	ret = vsprintf(buffer, format, aptr);
//	va_end(aptr);
//	HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),0xffff);
//	vPortFree(buffer);
//	//return ret;
//
//}


// uart process task
void uart_task(void *pvParameters) {
	uint8_t *buf;
	uint8_t len;
	int index;


	UART_Receieve_Init();
	while (1) {
		len = UART_Avaliable();
		if (len > 0) {
			buf = pvPortMalloc(len * sizeof(uint8_t));
			for (index = 0; index < len; index++) {
				buf[index] = UART_RxBuffer_read();
			}
			analysisCOMMAND(buf, len);
			vPortFree(buf);
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
