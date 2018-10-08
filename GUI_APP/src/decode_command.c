#include "decode_command.h"


//static uint8_t cmd_temp[HEADER_SIZE + MAX_DEVICE_SIZE *2]; //
//static uint8_t msg_temp[MAX_MSG_SIZE]; //
int device_number;


Device_Msg  msg_temp;

TaskHandle_t decodeTaskHandler;

void decode_command_task()
{
	uint8_t *read_temp_ptr;
	uint8_t adu_len;
	int index = 0;
	uint16_t tmp;
	while(1){
		while(commandBufferRxCount > 0) {
			read_temp_ptr = readCOMMANDData();
			adu_len = *(read_temp_ptr + CMD_ADU_LEN);
//			memcpy(cmd_temp, read_temp_ptr, adu_len);
			switch (*(read_temp_ptr + CMD_FC)) {
			case CMD_DEV_LIST:
				index = 0;
				device_number = ( adu_len - CONFIG_COMMAND_HEADER_SIZE) / 2;
				setNumberofDevice(device_number,0);
				break;
			case CMD_DEV_MSG:
				memcpy(&msg_temp, read_temp_ptr + CMD_START_DATA,CONFIG_MSG_DATA_SIZE);

				airdata_set_shortaddress(msg_temp.shortaddress, index);
				airdata_set_temperature(msg_temp.temperature, index);
				airdata_set_humidity(msg_temp.humidity, index);
				airdata_set_co2(msg_temp.co2, index);
				airdata_set_voc(msg_temp.voc, index);
				airdata_set_pm2_5(msg_temp.pm2_5, index);

				index++;
				if(index > device_number-1)index = 0;

				break;
			default:
				break;
			}
		}
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
