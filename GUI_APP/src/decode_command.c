#include "decode_command.h"
#include "uart.h"
#include "config.h"
#include "air_data.h"

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
	uint16_t tmp;
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
				tmp = msg_temp.ShortAddress[0];
				tmp = (tmp << 8) + msg_temp.ShortAddress[1];

				tmp = msg_temp.Temperature[0];
				tmp = (tmp << 8) + msg_temp.Temperature[1];
				setTemperature(tmp / 10.,index);

				tmp = msg_temp.Humidity[0];
				tmp = (tmp << 8) + msg_temp.Humidity[1];
				setHumdity(tmp / 10.,index);

				tmp = msg_temp.VOC[0];
				tmp = (tmp << 8) + msg_temp.VOC[1];
				setVoc(tmp / 1000.,index);

				tmp = msg_temp.CO2[0];
				tmp = (tmp << 8) + msg_temp.CO2[1];
				setCo2(tmp,index);

				tmp = msg_temp.PM2_5[0];
				tmp = (tmp << 8) + msg_temp.PM2_5[1];
				setPM2_5(tmp / 10.,index);

				index++;
				if(index > MAX_DEVICE_SIZE)index = 0;

				break;
			default:
				break;
			}
		}
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
