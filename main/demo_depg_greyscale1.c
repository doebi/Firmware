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
demoDepgGreyscale1(void) {
	/* first update */
	memset(screen_buf, 0x00, sizeof(screen_buf)/2);
	memset(&screen_buf[sizeof(screen_buf)/2], 0xff, sizeof(screen_buf)/2);

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

	/* second update */
	memset(screen_buf, 0x00, sizeof(screen_buf)/8);
	memset(&screen_buf[sizeof(screen_buf)/8], 0xff, sizeof(screen_buf)/8);
	memset(&screen_buf[2*sizeof(screen_buf)/8], 0x00, sizeof(screen_buf)/8);
	memset(&screen_buf[3*sizeof(screen_buf)/8], 0xff, sizeof(screen_buf)/8);
	memset(&screen_buf[4*sizeof(screen_buf)/8], 0x00, sizeof(screen_buf)/8);
	memset(&screen_buf[5*sizeof(screen_buf)/8], 0xff, sizeof(screen_buf)/8);
	memset(&screen_buf[6*sizeof(screen_buf)/8], 0x00, sizeof(screen_buf)/8);
	memset(&screen_buf[7*sizeof(screen_buf)/8], 0xff, sizeof(screen_buf)/8);

	badge_eink_display(screen_buf, DISPLAY_FLAG_NO_UPDATE|DISPLAY_FLAG_ALT_BUFFER);

	// null update
	const uint8_t lut_null[70] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
	};

	struct badge_eink_update eink_upd_2 = {
		.lut      = -1,
		.lut_custom = lut_null,
		.reg_0x3a = 26,   // 26 dummy lines per gate
		.reg_0x3b = 0x08, // 62us per line
		.y_start  = 0,
		.y_end    = 295,
	};
	badge_eink_update(&eink_upd_2);

	/* third update */
	memset(screen_buf, 0x00, sizeof(screen_buf)/4);
	memset(&screen_buf[sizeof(screen_buf)/4], 0xff, sizeof(screen_buf)/4);
	memset(&screen_buf[2*sizeof(screen_buf)/4], 0x00, sizeof(screen_buf)/4);
	memset(&screen_buf[3*sizeof(screen_buf)/4], 0xff, sizeof(screen_buf)/4);

	badge_eink_display(screen_buf, DISPLAY_FLAG_NO_UPDATE);

	// custom lut
	const uint8_t lut[70] = {
// white to black:
		0xa0, 0x90, 0x50, 0x00, 0x00, 0x00, 0x00, // group A (cycle 1..7)
// black to white:
		0x50, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, // group B
// white to black:
		0xa0, 0x90, 0x50, 0x00, 0x00, 0x00, 0x00, // group C
// black to white:
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // group D: null
		// 0x20 -> black to white: almost white
		// 0x80 -> black to white: almost white
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // VCOM

		// timings per cycle
// write inverse:
//		0x0f, 0x0f, 0x00, 0x00, 0x00, // A, B, C, D, repeat
		0x00, 0x00, 0x00, 0x00, 0x00,
// write all black:
//		0x0f, 0x0f, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x00, 0x00,
// write normal image:
//		0x0f, 0x0f, 0x00, 0x00, 0x00,
		0x01, 0x01, 0x00, 0x00, 0x00,
//		0x00, 0x00, 0x00, 0x00, 0x00,
// empty slots:
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
	};

	struct badge_eink_update eink_upd_3 = {
		.lut      = -1,
		.lut_custom = lut,
		.reg_0x3a = 26,   // 26 dummy lines per gate
		.reg_0x3b = 0x08, // 62us per line
		.y_start  = 0,
		.y_end    = 295,
	};
	badge_eink_update(&eink_upd_3);

	// wait for random keypress
	uint32_t buttons_down = 0;
	while ((buttons_down & 0xffff) == 0)
		xQueueReceive(evt_queue, &buttons_down, portMAX_DELAY);
}

#endif // CONFIG_SHA_BADGE_EINK_DEPG0290B1
