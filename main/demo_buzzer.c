#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <badge_buzzer.h>

void demoBuzzer(void)
{
	ets_printf("Buzz pattern 1\n");
	badge_buzzer_activate(0x777); // 111 0111 0111
	vTaskDelay(1000 / portTICK_PERIOD_MS);

	ets_printf("Buzz pattern 2\n");
	badge_buzzer_activate(0xdb); // 1101 1011
	vTaskDelay(1000 / portTICK_PERIOD_MS);

	ets_printf("Buzz pattern 3\n");
	badge_buzzer_activate(0x15); // 1 0101
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}
