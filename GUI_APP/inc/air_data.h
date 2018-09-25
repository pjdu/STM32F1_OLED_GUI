#ifndef _AIR_DATA_H_
#define _AIR_DATA_H_
#include <stdint.h>
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


typedef struct{
		union{
			uint16_t shortaddress;
			uint8_t ShortAddress[2];
		};

		union{
			  uint16_t temperature;
			  uint8_t  Temperature[2];
		};
		union{
			uint16_t  humidity;
			uint8_t   Humidity[2];
		};

		union{
		    uint16_t  voc;
			uint8_t   VOC[2];
		};
		union{
			uint16_t  co2;
			uint8_t   CO2[2];
		};
		union{
		    uint16_t  pm2_5;
			uint8_t   PM2_5[2];
		};
		uint16_t				  numberofDevice;
		uint16_t                  fan1Mode;
		uint16_t                  fan2Mode;
		uint16_t                  IAQMode;
}Device_Msg;


enum {
    CMD_STARCODE = 0,
    CMD_ADU_LEN,
    CMD_FC,
    CMD_START_DATA
};

enum{
	CMD_DEV_LIST = 0x02,
	CMD_DEV_MSG	 = 0x03,
};

void setNumberofDevice(uint16_t num, int device_number);
void setShortaddress(uint16_t shortaddress , int device_number);
void setTemperature(uint16_t temperature, int device_number);
void setHumdity(uint16_t humidity, int device_number);
void setCo2(uint16_t co2, int device_number);
void setVoc(uint16_t voc, int device_number);
void setPM2_5(uint16_t pm2_5, int device_number);
void setFan1Mode(uint16_t fan1Mode, int device_number);
void setFan2Mode(uint16_t fan2Mode, int device_number);
void setIAQMode(uint16_t IAQMode, int device_number);

uint16_t getNumberofDevice(int device_number);
uint16_t getTemperature(int device_number);
uint16_t getHumdity(int device_number);
uint16_t getCo2(int device_number);
uint16_t getVoc(int device_number);
uint16_t getPM2_5(int device_number);
uint16_t getFan1Mode(int device_number);
uint16_t getFan2Mode(int device_number);
uint16_t getIAQMode(int device_number);

#endif
