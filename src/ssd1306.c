#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <avr/pgmspace.h>

#include "i2c.h"
#include "img.h"

// Hard-coded display address:
#define SSD1306_ADDRESS			0x78

// Command registers:
#define SSD1306_SETCONTRAST 		0x81
#define SSD1306_DISPLAYALLON_RESUME 	0xA4
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_DISPLAYOFF 		0xAE
#define SSD1306_DISPLAYON 		0xAF
#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETCOMPINS 		0xDA
#define SSD1306_SETVCOMDETECT 		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETPRECHARGE 		0xD9
#define SSD1306_ENABLE_CHARGE_PUMP	0x8D
#define SSD1306_SETMULTIPLEX 		0xA8
#define SSD1306_SETSTARTLINE 		0x40
#define SSD1306_MEMORYMODE 		0x20
#define SSD1306_HV_COLUMN_ADDRESS	0x21
#define SSD1306_HV_PAGE_ADDRESS		0x22
#define SSD1306_PAM_PAGE_START		0xB0
#define SSD1306_COMSCANDEC 		0xC8
#define SSD1306_SEGREMAP 		0xA0

static void
write (const uint8_t *buf, size_t len)
{
	i2c_start();
	i2c_write(SSD1306_ADDRESS);

	while (len--)
		if (i2c_write(*buf++))
			break;

	i2c_stop();
}

static void
clear (void)
{
	i2c_start();
	i2c_write(SSD1306_ADDRESS);

	i2c_write(0x40);
	for (uint16_t i = 128 * 8; i; i--)
		i2c_write(0x00);

	i2c_stop();
}

void
ssd1306_show_img (void)
{
	i2c_start();
	i2c_write(SSD1306_ADDRESS);

	i2c_write(0x40);
	for (uint16_t i = sizeof(img); i; i--)
		i2c_write(pgm_read_byte(&img[i]));

	i2c_stop();
}

void
ssd1306_init (void)
{
	// Init sequence:
	const uint8_t seq[] = {
		0x00,
		SSD1306_DISPLAYOFF,
		SSD1306_SETDISPLAYCLOCKDIV,	0xF0,
		SSD1306_SETMULTIPLEX,		0x3F,
		SSD1306_SETPRECHARGE,		0xF1,
		SSD1306_SETDISPLAYOFFSET,	0x00,
		SSD1306_SETSTARTLINE,		0x00,
		SSD1306_ENABLE_CHARGE_PUMP,	0x14,
		SSD1306_MEMORYMODE,		0x00,
		SSD1306_SEGREMAP + 1,
		SSD1306_COMSCANDEC,
		SSD1306_SETCOMPINS,		0x10,
		SSD1306_SETCONTRAST,		0xCF,
		SSD1306_SETVCOMDETECT,		0x40,
		SSD1306_DISPLAYALLON_RESUME,
		SSD1306_NORMALDISPLAY,
		SSD1306_HV_COLUMN_ADDRESS,	0, 127,
		SSD1306_HV_PAGE_ADDRESS,	0, 7,
		SSD1306_DISPLAYON,
	};

	write(seq, sizeof(seq));

	// Clear display:
	clear();

	// Set window to image size:
	const uint8_t win[] = {
		0x00,
		SSD1306_HV_COLUMN_ADDRESS,	0, 127,
		SSD1306_HV_PAGE_ADDRESS,	2, 7,
	};

	write(win, sizeof(win));
}
