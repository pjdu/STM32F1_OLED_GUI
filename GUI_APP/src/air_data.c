#include "air_data.h"
#include "string.h"

static air_data_packet air_data = {};


void setTemperature(double temperature){
    air_data.temperature = temperature;
}

double getTemperature(){
    return air_data.temperature;
}

void setHumdity(double humidity){
    air_data.humidity = humidity;
}

double getHumdity(){
    return air_data.humidity;
}

void setCo2(double co2){
    air_data.co2 = co2;
}

double getCo2(){
    return air_data.co2;
}

void setVoc(double voc){
    air_data.voc = voc;
}

double getVoc(){
    return air_data.voc;
}

void setPM2_5(double pm2_5){
    air_data.pm2_5 = pm2_5;
}

double getPM2_5(){
    return air_data.pm2_5;
}

double getFan1Mode(){
    return air_data.fan1Mode;
}

void setFan1Mode(double fan1Mode){
    air_data.fan1Mode = fan1Mode;
}

double getFan2Mode(){
    return air_data.fan2Mode;
}

void setFan2Mode(double fan2Mode){
    air_data.fan2Mode = fan2Mode;
}

int getIAQMode(){
    return air_data.IAQMode;
}
void setIAQMode(int IAQMode){
    air_data.IAQMode = IAQMode;
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
