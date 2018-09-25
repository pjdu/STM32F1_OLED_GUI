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
				tmp = msg_temp.ShortAddress[0];
				tmp = (tmp << 8) + msg_temp.ShortAddress[1];
				msg_temp.shortaddress = tmp;

				tmp = msg_temp.Temperature[0];
				tmp = (tmp << 8) + msg_temp.Temperature[1];
				msg_temp.temperature = tmp;


				tmp = msg_temp.Humidity[0];
				tmp = (tmp << 8) + msg_temp.Humidity[1];
				msg_temp.humidity = tmp;

				tmp = msg_temp.VOC[0];
				tmp = (tmp << 8) + msg_temp.VOC[1];
				msg_temp.voc = tmp;

				tmp = msg_temp.CO2[0];
				tmp = (tmp << 8) + msg_temp.CO2[1];
				msg_temp.co2 = tmp;

				tmp = msg_temp.PM2_5[0];
				tmp = (tmp << 8) + msg_temp.PM2_5[1];
				msg_temp.pm2_5 = tmp;

				setShortaddress(msg_temp.shortaddress, index);
				setTemperature(msg_temp.temperature ,index);
				setHumdity(msg_temp.humidity ,index);
				setCo2(msg_temp.co2,index);
				setVoc(msg_temp.voc ,index);
				setPM2_5(msg_temp.pm2_5,index);

				index++;
				if(index > device_number)index = 0;

				break;
			default:
				break;
			}
		}
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
