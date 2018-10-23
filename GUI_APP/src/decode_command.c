#include "decode_command.h"
#include "rtc.h"

//static uint8_t cmd_temp[HEADER_SIZE + MAX_DEVICE_SIZE *2]; //
//static uint8_t msg_temp[MAX_MSG_SIZE]; //


TaskHandle_t decodeTaskHandler;

void decode_command_task()
{
	int device_number;
	Device_Msg  msg_temp;
	Time_Msg msg_time;
	uint8_t *read_temp_ptr;
	uint8_t adu_len;
	int index = 0;
	uint32_t commandlen;
	while(1){

		//接收通知，返回值為command的數量
		commandlen = ulTaskNotifyTake(pdTRUE, 	/*接收完通知時，將通知值清零*/
									  0       );/*沒收到通知,不花時間等待＝花0個tick時間等待*/
		while(commandlen > 0) {
			read_temp_ptr = readCOMMANDData();
			adu_len = *(read_temp_ptr + CMD_ADU_LEN);
			switch (*(read_temp_ptr + CMD_FC)) {
			case CMD_DEV_LIST:
				index = 0;
				device_number = ( adu_len - CONFIG_COMMAND_HEADER_SIZE) / 2;
				airdata_create_dev(device_number);
				airdata_set_numberofDevice(device_number,0);
				break;
			case CMD_DEV_MSG:
				memcpy(msg_temp.ShortAddress, read_temp_ptr + CMD_START_DATA,CONFIG_MSG_DATA_SIZE);

				airdata_set_shortaddress(msg_temp.shortaddress, index);
				airdata_set_temperature(msg_temp.temperature, index);
				airdata_set_humidity(msg_temp.humidity, index);
				airdata_set_co2(msg_temp.co2, index);
				airdata_set_voc(msg_temp.voc, index);
				airdata_set_pm2_5(msg_temp.pm2_5, index);

				index++;
				if(index > device_number-1)index = 0;

				break;
			case CMD_DEV_TIME:
				memcpy(msg_time.all_data, read_temp_ptr + CMD_START_DATA,CONFIG_TIME_DATA_SIZE);
				RTC_Set(msg_time.year,msg_time.mounth,msg_time.date, \
						msg_time.hour,msg_time.minute,msg_time.second);
				break;
			default:
				break;
			}
			commandlen--;
		}
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
