#ifndef _AIR_DATA_H_
#define _AIR_DATA_H_
#include <stdint.h>
typedef struct{
    double                  temperature;
    double                  humidity;
    double                  co2;
    double                  voc;
    double                  pm2_5;
    double                  fan1Mode;
    double                  fan2Mode;
    int                     IAQMode;
    char                    *updateTimeStamp;
    int                     len_updateTimeStamp;
}air_data_packet;


typedef struct{
	uint8_t ShortAddress[2];
	uint8_t Temperature[2];
	uint8_t Humidity[2];
	uint8_t VOC[2];
	uint8_t CO2[2];
	uint8_t PM2_5[2];
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

void setTemperature(double temperature, int device_number);
void setHumdity(double humidity, int device_number);
void setCo2(double co2, int device_number);
void setVoc(double voc, int device_number);
void setPM2_5(double pm2_5, int device_number);
void setFan1Mode(double fan1Mode, int device_number);
void setFan2Mode(double fan2Mode, int device_number);
void setIAQMode(int IAQMode, int device_number);

double getTemperature(int device_number);
double getHumdity(int device_number);
double getCo2(int device_number);
double getVoc(int device_number);
double getPM2_5(int device_number);
double getFan1Mode(int device_number);
double getFan2Mode(int device_number);
int getIAQMode(int device_number);

#endif
