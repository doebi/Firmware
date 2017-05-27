#include "sdkconfig.h"

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <esp_event.h>

#include <badge_eink.h>

#include "event_queue.h"

#ifdef CONFIG_SHA_BADGE_EINK_DEPG0290B1

// re-use screen_buf from main.c
extern uint8_t screen_buf[296*16];

void
demoDepgGreyscale2(void) {
	// use checkered board as reset
	int ch;
	for (ch=0; ch<2; ch++)
	{
		int y;
		for (y=0; y<128; y++)
		{
			memset(&screen_buf[y*296/8], ((y^ch) & 1) ? 0x55 : 0xaa, 296/8);
		}
		badge_eink_display(screen_buf, DISPLAY_FLAG_NO_UPDATE);
		struct badge_eink_update eink_upd_1 = {
			.lut      = 0,    // LUT_FULL
			.reg_0x3a = 26,   // 26 dummy lines per gate
			.reg_0x3b = 0x08, // 62us per line
			.y_start  = 0,
			.y_end    = 295,
		};
		badge_eink_update(&eink_upd_1);
	}

	/* initialize with black screen */
	memset(screen_buf, 0x00, sizeof(screen_buf));

	badge_eink_display(screen_buf, DISPLAY_FLAG_NO_UPDATE);

	// default update
	struct badge_eink_update eink_upd_1 = {
		//.lut      = LUT_DEFAULT,
		.lut      = 0,
		.reg_0x3a = 26,   // 26 dummy lines per gate
		.reg_0x3b = 0x08, // 62us per line
		.y_start  = 0,
		.y_end    = 295,
	};
	badge_eink_update(&eink_upd_1);

	/* partial updates */
	int i;
	for (i=0; i<5; i++)
	{
		int y;
		for (y=0; y<128; y++)
		{
			memset(&screen_buf[y*296/8], ((y >> (2+i)) & 1) ? 0xff : 0x00, 296/8);
		}

		badge_eink_display(screen_buf, DISPLAY_FLAG_NO_UPDATE);

		// custom lut
		const uint8_t lut[70] = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // group A (cycle 1..7)
			0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // group B
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // group C
			0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // group D
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // VCOM

			// timings per cycle
			1<<i, 0x00, 0x00, 0x00, 0x00,
		// empty slots:
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		};

		struct badge_eink_update eink_upd_3 = {
			.lut      = -1,
			.lut_custom = lut,
			.reg_0x3a = 0,
			.reg_0x3b = 3,
			.y_start  = 0,
			.y_end    = 295,
		};
		badge_eink_update(&eink_upd_3);
	}

	// wait for random keypress
	uint32_t buttons_down = 0;
	while ((buttons_down & 0xffff) == 0)
		xQueueReceive(evt_queue, &buttons_down, portMAX_DELAY);
}

#endif // CONFIG_SHA_BADGE_EINK_DEPG0290B1
