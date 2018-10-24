#ifndef __CONFIG_H
#define __CONFIG_H



#define DEBUG  		 0		 //Debug = 1時，uart接收會被鎖住，用於除錯模式會有記憶體剩餘訊息從uart輸出
							 //Debug = 0時，	會關閉錯誤訊息輸出


/* 任務名稱		    運行狀態	 優先級	 	剩餘堆疊			任務編號
 * LIST_task          	X		10			12			10
 * main task          	R		2			97			5
 * Menu_Task          	B		2			50			12
 * RTCUI_Task         	B		3			114			18
 * SENSOR_UI_Task     	B		3			182			22
 * RTCUI_Task         	B		3			114			34
 * Button_task        	R		1			43			4
 * IDLE               	R		0			68			2
 * decode_task        	B		5			1115		7
 * uart_task          	B		4			1118		6
 * iwdg_Task          	B		10			47			9
 * GPIO_task          	B		6			11			8
 * Tmr Svc            	B		3			58			3
 *
 * Toatal usage								4 * sum()
 *
 * Free Heap size = 1104
 * */

#define SUPPORT_OS   1 		//1有使用OS,delay_ms採用os版本的vTaskDelay
							//0不使用os,delay_ms採用一般版本dwt_delay


//FreeRTOS 使用之HEAP SIZE ，初始化需使用約7K，穩定運行狀態大約使用5K
//應用若增加最大約可調至20K，此區記憶體占用可在Build Analyzer觀察RAM的編譯時期使用率
#define CONFIG_TOTAL_HEAP_SIZE ( ( size_t ) ( 12 * 1024 ) + (CONFIG_MAX_COMMAND_SIZE * CONFIG_SEQ_COMMAND_BUFFER_SIZE) / 4 )


//CONFIG_FOR_COMMAND
#define CONFIG_COMMAND_HEADER_SIZE 	      4 	// HEADER + LENGTH + FC + CS = 4Byte


//增加下面兩個的大小，會連動增加下面兩個TASK大小
/*
 *#define CONFIG_TOTAL_HEAP_SIZE
 *#define CONFIG_UART_TASK_STACK_SIZE 				128		// Task Stack Size
 *#define CONFIG_DECODE_TASK_STACK_SIZE 			128		// Task Stack Size
 *
 *uart 最大緩充區256 byte
 *最大device個數  = ( CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE ) / 2
 *			    =   (128 - 4) / 2 = 62
 *
*/
#define CONFIG_MAX_COMMAND_SIZE 		  128   // 每一條COMMAND 最大大小
#define CONFIG_SEQ_COMMAND_BUFFER_SIZE    16   // 最大儲存COMMAND的個數



#define CONFIG_MSG_DATA_SIZE    	     12
#define CONFIG_TIME_DATA_SIZE			  7

#define CONFIG_MAX_DEVICE_SIZE  		 (CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE) >> 1


//EventGroupHandle_t EventGroupHandler in main.c
//CONFIG FOR HANDLE EVENT
#define NULL_EVENT_RAISE 			 0
#define BUTTON_PRESS_EVENT 			(1 << 0)
#define BUTTON_PRESS_1S_EVENT 		(1 << 1)
#define ROTATE_EVENT                (1 << 2)


//使用到作業系統之任務=>統稱為APP


//	CONFIG FOR APP TASK PRIORITY
//	優先級越高越先處理
#define CONFIG_LIST_TASK_PRIORITY					1
#define CONFIG_START_TASK_PRIORITY   				1  		// Task Priority
#define CONFIG_BUTTON_TASK_PRIORITY   				1  		// Task Priority

#define CONFIG_MAIN_UI_PAGE_TASK_PRIORITY   		2  		// Task Priority
#define CONFIG_MENU_TASK_PRIORITY   				2  		// Task Priority

#define CONFIG_RTCUI_TASK_PRIORITY   				3  		// Task Priority
#define CONFIG_SENSOR_UI_TASK_PRIORITY   			3  		// Task Priority

#define CONFIG_UART_TASK_PRIORITY   				4  	 	// Task Priority
#define CONFIG_DECODE_TASK_PRIORITY   				5		// Task Priority
#define CONFIG_GPIO_TASK_PRIORITY   				6  		// Task Priority
#define CONFIG_IWDG_TASK_PRIORITY   				10  	// Task Priority







//CONFIG FOR APP STACK SIZE USE
#define CONFIG_START_TASK_STACK_SIZE 				128		// Task Stack Size
#define CONFIG_LIST_TASK_STACK_SIZE 				320
#define CONFIG_BUTTON_TASK_STACK_SIZE 				70		// Task Stack Size
#define CONFIG_MAIN_UI_PAGE_TASK_STACK_SIZE 		256		// Task Stack Size
#define CONFIG_MENU_TASK_STACK_SIZE 				128		// Task Stack Size
#define CONFIG_RTCUI_TASK_STACK_SIZE 				256		// Task Stack Size
#define CONFIG_SENSOR_UI_TASK_STACK_SIZE 			256		// Task Stack Size

#define CONFIG_UART_TASK_STACK_SIZE 				(128 + (CONFIG_MAX_COMMAND_SIZE * CONFIG_SEQ_COMMAND_BUFFER_SIZE) / 16)	// Task Stack Size
#define CONFIG_DECODE_TASK_STACK_SIZE 				(128 + (CONFIG_MAX_COMMAND_SIZE * CONFIG_SEQ_COMMAND_BUFFER_SIZE) / 16)	// Task Stack Size

#define CONFIG_GPIO_TASK_STACK_SIZE 				120		// Task Stack Size
#define CONFIG_IWDG_TASK_STACK_SIZE 				70		// Task Stack Size






#endif
