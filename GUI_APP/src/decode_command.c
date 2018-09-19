#include "decode_command.h"

static uint8_t cmd_temp[16];
Device_Msg dev_array[10];

void decode_command_task()
{
	uint8_t *read_temp_ptr;
	uint8_t adu_len;
	while(1){
		while(commandBufferRxCount > 0) {
			read_temp_ptr = readCOMMANDData();
			adu_len = *(read_temp_ptr + CMD_ADU_LEN);
			memcpy(cmd_temp, read_temp_ptr, adu_len);
			switch (*(read_temp_ptr + CMD_FC)) {
			case CMD_DEV_LIST:
				break;
			case CMD_DEV_MSG:
				break;
			default:
				break;
			}
		}
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}
