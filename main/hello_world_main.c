/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
//Este codigo crea diferentes tareas usando la libreria task.h en cada tarea se ejecuta un bloque independiente
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
static const char *TAG = "CODE";
//Distintas tareas 
void Tarea1(void *pvParameters);
void Tarea2(void *pvParameters);
void Tarea3(void *pvParameters);

void app_main(void)
{
    TaskHandle_t xHandle = NULL;
    ESP_LOGI(TAG,"Hello dasd!\n");
    //Creamos las tareas 
    xTaskCreate(Tarea1,"Tarea1",sizeof(int16_t),NULL,1,&xHandle);
    xTaskCreate(Tarea2,"Tarea2",sizeof(int16_t),NULL,2,&xHandle);
    xTaskCreate(Tarea3,"Tarea3",sizeof(int16_t),NULL,3,&xHandle);
    
    ESP_LOGI(TAG,"Tareas creadas!\n");
}
    void Tarea1(void *pvParameters){
    char text[25]= "Hola desde la tarea 1";
    while(1){
      vTaskDelay(pdMS_TO_TICKS(500));
      ESP_LOGI(TAG,"%s",text);
      }
    }   

void Tarea2(void *pvParameters){
    char text[25]= "Hola desde la tarea 2";
    while(1){
      vTaskDelay(pdMS_TO_TICKS(100));
      ESP_LOGI(TAG,"%s",text);
      }
    }   

void Tarea3(void *pvParameters){
    char text[25]= "Hola desde la tarea 3";
    while(1){
      vTaskDelay(pdMS_TO_TICKS(1500));
      ESP_LOGI(TAG,"%s",text);
      }
    }   