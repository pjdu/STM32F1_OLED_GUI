#include "air_data.h"
#include "string.h"
#include "config.h"

static air_data_packet air_data[MAX_DEVICE_SIZE];


void setTemperature(double temperature , int device_number){
    air_data[device_number].temperature = temperature;
}

void setHumdity(double humidity ,int device_number){
    air_data[device_number].humidity = humidity;
}

void setCo2(double co2,int device_number){
    air_data[device_number].co2 = co2;
}

void setVoc(double voc,int device_number){
    air_data[device_number].voc = voc;
}

void setPM2_5(double pm2_5,int device_number){
    air_data[device_number].pm2_5 = pm2_5;
}

void setFan1Mode(double fan1Mode,int device_number){
    air_data[device_number].fan1Mode = fan1Mode;
}

void setFan2Mode(double fan2Mode,int device_number){
    air_data[device_number].fan2Mode = fan2Mode;
}
void setIAQMode(int IAQMode,int device_number){
    air_data[device_number].IAQMode = IAQMode;
}


double getTemperature(int device_number){
    return air_data[device_number].temperature;
}

double getHumdity(int device_number){
    return air_data[device_number].humidity;
}

double getCo2(int device_number){
    return air_data[device_number].co2;
}

double getVoc(int device_number){
    return air_data[device_number].voc;
}

double getPM2_5(int device_number){
    return air_data[device_number].pm2_5;
}

double getFan1Mode(int device_number){
    return air_data[device_number].fan1Mode;
}

double getFan2Mode(int device_number){
    return air_data[device_number].fan2Mode;
}
int getIAQMode(int device_number){
    return air_data[device_number].IAQMode;
}
// //以下函數需成對呼叫，否則會有記憶體洩漏的問題
// void getTimeStamp(char *TimeStamp){
//     air_data.len_updateTimeStamp = strlen(air_data.updateTimeStamp);
//     memcpy(TimeStamp,air_data.updateTimeStamp,air_data.len_updateTimeStamp);
//     free(air_data.updateTimeStamp);
// }

// void setTimeStamp(char *TimeStamp){
//     air_data.updateTimeStamp = malloc(sizeof(char)*strlen(TimeStamp));
//     memcpy(air_data.updateTimeStamp,TimeStamp,strlen(TimeStamp));
// }
