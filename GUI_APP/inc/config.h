#ifndef __CONFIG_H
#define __CONFIG_H



#define DEBUG  		 0		 //Debug = 1時，用於除錯模式會有記憶體剩餘訊息從uart輸出
							 //Debug = 0時，	會關閉錯誤訊息輸出

#define SUPPORT_OS   1 		//1有使用OS,delay_ms採用os版本的vTaskDelay
							//0不使用os,delay_ms採用一般版本dwt_delay


//FreeRTOS 使用之HEAP SIZE ，初始化需使用約7K，穩定運行狀態大約使用5K
//應用若增加最大約可調至20K，此區記憶體占用可在Build Analyzer觀察RAM的編譯時期使用率
#define CONFIG_TOTAL_HEAP_SIZE ( ( size_t ) ( 10 * 1024 ) )


//CONFIG_FOR_COMMAND
#define CONFIG_COMMAND_HEADER_SIZE 	      4
#define CONFIG_MAX_COMMAND_SIZE 		  16
#define CONFIG_MSG_DATA_SIZE    	     (CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE)
#define CONFIG_SEQ_COMMAND_BUFFER_SIZE    16
#define CONFIG_MAX_DEVICE_SIZE  		 (CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE) >> 1


//EventGroupHandle_t EventGroupHandler in main.c
//CONFIG FOR HANDLE EVENT
#define NULL_EVENT_RAISE 			 0
#define BUTTON_PRESS_EVENT 			(1 << 0)
#define BUTTON_PRESS_1S_EVENT 		(1 << 1)
#define ROTATE_EVENT                (1 << 2)


//使用到作業系統之任務=>統稱為APP


//CONFIG FOR APP TASK PRIORITY
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
#define CONFIG_BUTTON_TASK_STACK_SIZE 				70		// Task Stack Size
#define CONFIG_MAIN_UI_PAGE_TASK_STACK_SIZE 		256		// Task Stack Size
#define CONFIG_MENU_TASK_STACK_SIZE 				128		// Task Stack Size
#define CONFIG_RTCUI_TASK_STACK_SIZE 				256		// Task Stack Size
#define CONFIG_SENSOR_UI_TASK_STACK_SIZE 			256		// Task Stack Size
#define CONFIG_UART_TASK_STACK_SIZE 				128		// Task Stack Size
#define CONFIG_DECODE_TASK_STACK_SIZE 				128		// Task Stack Size
#define CONFIG_GPIO_TASK_STACK_SIZE 				100		// Task Stack Size
#define CONFIG_IWDG_TASK_STACK_SIZE 				70		// Task Stack Size







#endif
