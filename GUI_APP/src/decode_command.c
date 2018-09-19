#include "decode_command.h"


#define MAX_DEVICE_SIZE 6
#define HEADER_SIZE 4  //4: Header + Length + FC + CS = 4byte
#define MAX_MSG_SIZE 16

#define MSG_DATA_SIZE (MAX_MSG_SIZE - HEADER_SIZE)

//static uint8_t cmd_temp[HEADER_SIZE + MAX_DEVICE_SIZE *2]; //
//static uint8_t msg_temp[MAX_MSG_SIZE]; //
int device_number;

Device_Msg dev_array[MAX_DEVICE_SIZE];
Device_Msg  msg_temp;

TaskHandle_t decodeTaskHandler;

void decode_command_task()
{
	uint8_t *read_temp_ptr;
	uint8_t adu_len;
	int index = 0;
	while(1){
		while(commandBufferRxCount > 0) {
			read_temp_ptr = readCOMMANDData();
			adu_len = *(read_temp_ptr + CMD_ADU_LEN);
//			memcpy(cmd_temp, read_temp_ptr, adu_len);
			switch (*(read_temp_ptr + CMD_FC)) {
			case CMD_DEV_LIST:
				index = 0;
				device_number = ( adu_len - HEADER_SIZE ) / 2;
				break;
			case CMD_DEV_MSG:
				memcpy(&msg_temp, read_temp_ptr + CMD_START_DATA,MSG_DATA_SIZE);
				uint16_t tmp = msg_temp.ShortAddress[0];
				tmp = (tmp << 8) + msg_temp.ShortAddress[1];

				tmp = msg_temp.Temperature[0];
				tmp = (tmp << 8) + msg_temp.Temperature[1];

				dev_array[index++] = msg_temp;
				if(index > MAX_DEVICE_SIZE)index = 0;

				break;
			default:
				break;
			}
		}
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}
