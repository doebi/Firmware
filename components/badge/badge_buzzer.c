#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "rom/ets_sys.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "badge_pins.h"
#include "badge_i2c.h"
#include "badge_portexp.h"
#include "badge_mpr121.h"
#include "badge_buzzer.h"

#if defined(PORTEXP_PIN_NUM_BUZZER) || defined(MPR121_PIN_NUM_BUZZER)

static int
badge_buzzer_on(void)
{
#ifdef PORTEXP_PIN_NUM_BUZZER
	return badge_portexp_set_output_state(PORTEXP_PIN_NUM_BUZZER, 1);
#elif defined(MPR121_PIN_NUM_BUZZER)
	return badge_mpr121_set_gpio_level(MPR121_PIN_NUM_BUZZER, 1);
#endif
}

static int
badge_buzzer_off(void)
{
#ifdef PORTEXP_PIN_NUM_BUZZER
	return badge_portexp_set_output_state(PORTEXP_PIN_NUM_BUZZER, 0);
#elif defined(MPR121_PIN_NUM_BUZZER)
	return badge_mpr121_set_gpio_level(MPR121_PIN_NUM_BUZZER, 0);
#endif
}

void
badge_buzzer_activate(int pattern)
{
	while (pattern != 0)
	{
		if ((pattern & 1) == 0)
			badge_buzzer_off();
		else
			badge_buzzer_on();
		pattern >>= 1;
		ets_delay_us(200000);
	}
	badge_buzzer_off();
}

void
badge_buzzer_init(void)
{
	// configure buzzer pin
#ifdef PORTEXP_PIN_NUM_BUZZER
	badge_portexp_set_output_state(PORTEXP_PIN_NUM_BUZZER, 0);
	badge_portexp_set_output_high_z(PORTEXP_PIN_NUM_BUZZER, 0);
	badge_portexp_set_io_direction(PORTEXP_PIN_NUM_BUZZER, 1);
#elif defined(MPR121_PIN_NUM_BUZZER)
	badge_mpr121_configure_gpio(MPR121_PIN_NUM_BUZZER, MPR121_OUTPUT);
#endif
}

#endif // defined(PORTEXP_PIN_NUM_BUZZER) || defined(MPR121_PIN_NUM_BUZZER)
