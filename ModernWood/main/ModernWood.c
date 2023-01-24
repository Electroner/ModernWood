#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//For dissabling wifi
#include "esp_wifi.h"

//For GPIO
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

//For BLE
#include "esp_gap_ble_api.h"

//For USB
#include "tinyusb.h"
#include "class/hid/hid_device.h"

//For logging
#include "esp_log.h"

void app_main()
{
	//Disable wifi
	esp_wifi_stop();

	//Blink LED attached to GPIO 11
	gpio_set_direction(GPIO_NUM_11, GPIO_MODE_OUTPUT);
	while(1)
	{
		gpio_set_level(11, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(11, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

//TODO: Select the COM port that the board is connected to and flash the code to the board