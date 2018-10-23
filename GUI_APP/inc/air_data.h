#ifndef _AIR_DATA_H_
#define _AIR_DATA_H_
#include <stdint.h>
#include "config.h"

#if SUPPORT_OS
	#include "FreeRTOS.h"
	#include "task.h"
#endif
//typedef struct{
//	uint16_t               	shortaddress;
//    double                  temperature;
//    double                  humidity;
//    double                  co2;
//    double                  voc;
//    double                  pm2_5;
//
//  //  char                    *updateTimeStamp;
//  // int                     len_updateTimeStamp;
//}air_data_packet;


typedef struct {

	union {
		uint8_t ShortAddress[2];
		uint16_t shortaddress;
	};
	union {
		uint8_t all_data[10];
		struct {
			union {
				uint8_t Temperature[2];
				uint16_t temperature;
			};
			union {
				uint8_t Humidity[2];
				uint16_t humidity;

			};

			union {
				uint8_t VOC[2];
				uint16_t voc;
			};
			union {
				uint8_t CO2[2];
				uint16_t co2;
			};
			union {
				uint8_t PM2_5[2];
				uint16_t pm2_5;
			};
		};
	};
	uint16_t numberofDevice;
//	uint16_t fan1Mode;
//	uint16_t fan2Mode;
//	uint16_t IAQMode;
} Device_Msg;

typedef struct {
	union {
		uint8_t all_data[7];
		struct{
			union{
				uint8_t YEAR[2];
				uint16_t year;
			};
			uint8_t mounth;
			uint8_t date;
			uint8_t hour;
			uint8_t minute;
			uint8_t second;
		};
	};
}Time_Msg;

enum {
    CMD_STARCODE = 0,
    CMD_ADU_LEN,
    CMD_FC,
    CMD_START_DATA
};

enum{
	CMD_DEV_LIST = 0x02,
	CMD_DEV_MSG	 = 0x03,
	CMD_DEV_TIME = 0x04
};


#define AIR_DECLARE_SET_MEMBER_FUNC_DEF(member) \
    void airdata_set_##member(uint16_t data, int device_number);

#define AIR_DECLARE_GET_MEMBER_FUNC_DEF(member) \
		uint16_t airdata_get_##member(int device_number);

void airdata_delete_dev();
void airdata_create_dev(int NumberOfDev);

AIR_DECLARE_SET_MEMBER_FUNC_DEF(shortaddress)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(shortaddress)

AIR_DECLARE_SET_MEMBER_FUNC_DEF(temperature)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(temperature)

AIR_DECLARE_SET_MEMBER_FUNC_DEF(humidity)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(humidity)

AIR_DECLARE_SET_MEMBER_FUNC_DEF(voc)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(voc)

AIR_DECLARE_SET_MEMBER_FUNC_DEF(co2)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(co2)

AIR_DECLARE_SET_MEMBER_FUNC_DEF(pm2_5)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(pm2_5)


AIR_DECLARE_SET_MEMBER_FUNC_DEF(numberofDevice)
AIR_DECLARE_GET_MEMBER_FUNC_DEF(numberofDevice)


#endif
