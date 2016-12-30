#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include <gde.h>
#include <gdeh029a1.h>
#include <pictures.h>

esp_err_t event_handler(void *ctx, system_event_t *event) { return ESP_OK; }

void app_main(void) {
  nvs_flash_init();
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config = {.sta = {.ssid = "access_point_name",
                                      .password = "password",
                                      .bssid_set = false}};
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());

  initSPI();
  initDisplay(false);
  displayImage(pictures[0], false);

  bool faster = false;

  int blink = 0;
  gpio_set_direction(GPIO_NUM_22, GPIO_MODE_OUTPUT);

  while (true) {
    vTaskDelay((faster ? 50 : 2000) / portTICK_PERIOD_MS);
    for (unsigned int picture = 1; picture < NUM_PICTURES; picture++) {
      displayImage(pictures[picture], false);
      gpio_set_level(GPIO_NUM_22, blink);
      blink = !blink;
      vTaskDelay((faster ? 50 : 2000) / portTICK_PERIOD_MS);
    }
    if (!faster) {
    faster = !faster;
    initDisplay(faster);
    }
    displayImage(pictures[0], false);
    gpio_set_level(GPIO_NUM_22, blink);
    blink = !blink;
  }
}
