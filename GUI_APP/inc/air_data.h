#ifndef _AIR_DATA_H_
#define _AIR_DATA_H_
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


void setTemperature(double temperature);
void setHumdity(double humidity);
void setCo2(double co2);
void setVoc(double voc);
void setPM2_5(double pm2_5);
void setFan1Mode(double fan1Mode);
void setFan2Mode(double fan2Mode);
void setIAQMode(int IAQMode);

double getTemperature();
double getHumdity();
double getCo2();
double getVoc();
double getPM2_5();
double getFan1Mode();
double getFan2Mode();
int getIAQMode();

#endif
