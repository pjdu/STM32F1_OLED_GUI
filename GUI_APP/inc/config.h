#ifndef __CONFIG_H
#define __CONFIG_H

//CONFIG_FOR_COMMAND

#define DEBUG  1


#define CONFIG_COMMAND_HEADER_SIZE 	      4
#define CONFIG_MAX_COMMAND_SIZE 		  16//256
#define CONFIG_MSG_DATA_SIZE    	     (CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE)
#define CONFIG_SEQ_COMMAND_BUFFER_SIZE   16
#define CONFIG_MAX_DEVICE_SIZE  		 (CONFIG_MAX_COMMAND_SIZE - CONFIG_COMMAND_HEADER_SIZE) >> 1



//EventGroupHandle_t EventGroupHandler in main.c

//Handle event
#define NULL_EVENT_RAISE 			 0
#define BUTTON_PRESS_EVENT 			(1 << 0)
#define BUTTON_PRESS_1S_EVENT 		(1 << 1)
#define ROTATE_EVENT                (1 << 2)

#endif
