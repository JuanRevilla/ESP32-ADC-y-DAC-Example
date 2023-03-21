/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
//Esta tarea se basa en crear una onda senoidal en el puerto 25 y leerla en el puerto 32
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/dac_common.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
static const char *TAG = "CODE";
#define OUT 25
#define IN 32
//Definimos los perifericos.
adc_oneshot_unit_handle_t  hest = NULL;

//
esp_err_t ConfigOnda(void);
esp_err_t ConfigLect(void);

void app_main(void)
{
  int i=0;
  int L=0;
    ConfigOnda();
    ConfigLect();
    ESP_LOGI(TAG,"Hello world!\n");
    while(1){
      adc_oneshot_read(hest, ADC_CHANNEL_4, &L);
      ESP_LOGI(TAG,"Dato=%i en %i t",L, i);
      vTaskDelay(1000/portTICK_PERIOD_MS);
      i++;
    }

}
esp_err_t ConfigOnda(void){
  dac_pad_get_io_num(DAC_CHANNEL_1, OUT);
  dac_output_enable(DAC_CHANNEL_1);
  dac_cw_generator_enable();
  dac_cw_config_t Onda = {
    .en_ch=DAC_CHANNEL_1,
    .scale=DAC_CW_SCALE_8,
    .phase=DAC_CW_PHASE_0,
    .freq= 1000,
    .offset= 400,
  };
  dac_cw_generator_config(&Onda);
  return ESP_OK;
}
esp_err_t ConfigLect(){

const adc_oneshot_unit_init_cfg_t inicial ={
  .unit_id=ADC_UNIT_1,
  .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_chan_cfg_t canal ={
   .atten=ADC_ATTEN_DB_0,              ///< ADC attenuation
  .bitwidth=ADC_BITWIDTH_10,   
};
adc_oneshot_new_unit(&inicial, &hest);
adc_oneshot_config_channel(hest, ADC_CHANNEL_4, &canal);
adc_oneshot_io_to_channel(IN, ADC_UNIT_1, ADC_CHANNEL_4);
adc_oneshot_channel_to_io(ADC_UNIT_1, ADC_CHANNEL_4, IN);
  return ESP_OK;
}