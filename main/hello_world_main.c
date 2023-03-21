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
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "driver/ledc.h"

static const char *TAG = "CODE";
//Distintas tareas 
void Tarea1(void *pvParameters);
void Tarea2(void *pvParameters);
void Tarea3(void *pvParameters);
static bool cuenta(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx){
     BaseType_t high_task_wakeup;
     QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send event data to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);


}

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
    //Esta tarea se ejecuta en bucle y se encarga de generar la onda PWM
    while(1){
      vTaskDelay(pdMS_TO_TICKS(500));
      ESP_LOGI(TAG,"%s",text);
      }
    }   

void Tarea2(void *pvParameters){
    //Esta tarea se ejecuta en bucle y se encarga de contar pulsos
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI(TAG, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI(TAG, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI(TAG, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
    int pasos=0;
    while(1){
        if (xQueueReceive(queue, &pasos, pdMS_TO_TICKS(1000))) {
            ESP_LOGI(TAG, "Watch point event, count: %d", pasos);
        }
      }
    }   

void Tarea3(void *pvParameters){
    /* ESta tarea se ejecuta una vez y configura puertos y timers*/
    QueueHandle_t queue = xQueueCreate(10, sizeof(int));
      pcnt_unit_handle_t UniCont = NULL;
      pcnt_chan_t CanalCont = NULL;
    pcnt_channel_handle_t CanaHand = NULL;
     
      pcnt_watch_event_data_t Ojo = {
        .watch_point_value= 100,
        .zero_cross_mode= PCNT_UNIT_ZERO_CROSS_POS_ZERO,
      }
        const  pcnt_event_callbacks_t cbc={
        .on_reach=cuenta,
      }
      pcnt_unit_config_t UniConfig={
        .low_limit= -100,
        .high_limit= 100,
      }
      pcnt_chan_config_t CanalConfig={
        .edge_gpio_num = 33,
        .level_gpio_num = 32,
      }
      pcnt_glitch_filter_config_t MaxGlitch ={
        .max_glitch_ns= 1000,
      }
     ESP_ERROR_CHECK(pcnt_new_unit(&UniCOnfig, &UniCont));
     ESP_ERROR_CHECK(pcnt_new_channel(UniCont,&CanalConfig,,&CanaHand));
     ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(UniCont,&MaxGlitch));
     int watch_points[] = {-100, -50, 0, 50, 100};
     for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(UniCont, watch_points[i]));
        }
     ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(UniCont, &cbc, queue));
     ESP_ERROR_CHECK(pcnt_channel_set_edge_action(CanaHand, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
     ESP_ERROR_CHECK(pcnt_channel_set_level_action(CanaHand, PCNT_CHANNEL_EDGE_ACTION_HOLD, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    }   