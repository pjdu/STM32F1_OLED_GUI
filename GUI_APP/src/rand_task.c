#include "air_data.h"
#include "rand_task.h"
#include "stdlib.h"
#include"time.h"


TaskHandle_t randTaskHandler;

void rand_task()
{
    while(1){

//    	srand(xTaskGetTickCount());
//        setTemperature(rand() % 100);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setHumdity(rand() % 100);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setCo2(rand() % 50);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setVoc(rand() % 30);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setPM2_5(rand() % 200);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setFan1Mode(rand() % 2);
//        vTaskDelay(100/portTICK_PERIOD_MS);
//
//        srand(xTaskGetTickCount());
//        setFan2Mode(rand() % 2);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
