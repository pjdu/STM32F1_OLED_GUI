#include "air_data.h"
#include "string.h"
#include "config.h"

static Device_Msg dev_array[CONFIG_MAX_DEVICE_SIZE];

//static air_data_packet air_data[CONFIG_MAX_DEVICE_SIZE];

void setNumberofDevice(uint16_t num, int device_number)
{
	dev_array[device_number].numberofDevice = num;
}
void setShortaddress(uint16_t shortaddress , int device_number){
	 dev_array[device_number].shortaddress = shortaddress;
}
void setTemperature(uint16_t temperature , int device_number){
    dev_array[device_number].temperature = temperature;
}

void setHumdity(uint16_t humidity ,int device_number){
    dev_array[device_number].humidity = humidity;
}

void setCo2(uint16_t co2,int device_number){
    dev_array[device_number].co2 = co2;
}

void setVoc(uint16_t voc,int device_number){
    dev_array[device_number].voc = voc;
}

void setPM2_5(uint16_t pm2_5,int device_number){
    dev_array[device_number].pm2_5 = pm2_5;
}

void setFan1Mode(uint16_t fan1Mode,int device_number){
    dev_array[device_number].fan1Mode = fan1Mode;
}

void setFan2Mode(uint16_t fan2Mode,int device_number){
    dev_array[device_number].fan2Mode = fan2Mode;
}
void setIAQMode(uint16_t IAQMode,int device_number){
    dev_array[device_number].IAQMode = IAQMode;
}



uint16_t getNumberofDevice(int device_number)
{
	return dev_array[device_number].numberofDevice;
}

uint16_t getShortaddress(int device_number){
	return dev_array[device_number].shortaddress;
}

uint16_t getTemperature(int device_number){
    return dev_array[device_number].temperature;
}

uint16_t getHumdity(int device_number){
    return dev_array[device_number].humidity;
}

uint16_t getCo2(int device_number){
    return dev_array[device_number].co2;
}

uint16_t getVoc(int device_number){
    return dev_array[device_number].voc;
}

uint16_t getPM2_5(int device_number){
    return dev_array[device_number].pm2_5;
}

uint16_t getFan1Mode(int device_number){
    return dev_array[device_number].fan1Mode;
}

uint16_t getFan2Mode(int device_number){
    return dev_array[device_number].fan2Mode;
}
uint16_t getIAQMode(int device_number){
    return dev_array[device_number].IAQMode;
}
// //以下函數需成對呼叫，否則會有記憶體洩漏的問題
// void getTimeStamp(char *TimeStamp){
//     dev_array.len_updateTimeStamp = strlen(dev_array.updateTimeStamp);
//     memcpy(TimeStamp,dev_array.updateTimeStamp,dev_array.len_updateTimeStamp);
//     free(dev_array.updateTimeStamp);
// }

// void setTimeStamp(char *TimeStamp){
//     dev_array.updateTimeStamp = malloc(sizeof(char)*strlen(TimeStamp));
//     memcpy(dev_array.updateTimeStamp,TimeStamp,strlen(TimeStamp));
// }
