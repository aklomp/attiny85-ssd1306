#include <stdbool.h>
#include <avr/io.h>

#define PIN_SDA	0
#define PIN_SCL	2

static inline void
scl_release (void)
{
	PORTB |= _BV(PIN_SCL);
}

static inline void
scl_low (void)
{
	PORTB &= ~_BV(PIN_SCL);
}

static inline void
sda_release (void)
{
	// SDA is linked to the MSB of the data register:
	USIDR |= 0x80;
}

static inline void
sda_low (void)
{
	USIDR &= ~0x80;
}

// Send start condition
void
i2c_start (void)
{
	scl_release();
	sda_low();
	scl_low();
	sda_release();

	USISR = _BV(USISIF);
}

// Send stop condition
void
i2c_stop (void)
{
	sda_low();
	scl_release();
	sda_release();

	USISR = _BV(USIPF);
}

// Write a byte, return ack status
bool
i2c_write (uint8_t c)
{
	USIDR = c;
	USISR = 7;	// 16 - 7 = 9 bits

	do {
		scl_release();
		scl_low();
		USICR |= _BV(USICLK);
	}
	while (USISR & 0x0F);

	sda_release();
	return USIDR & 0x01;
}

void
i2c_init (void)
{
	// Enable pullups and set as outputs:
	PORTB = _BV(PIN_SDA) | _BV(PIN_SCL);
	DDRB  = _BV(PIN_SDA) | _BV(PIN_SCL);

	// Two-wire mode with software-defined clock:
	USICR = _BV(USIWM1) | _BV(USICLK);

	scl_release();
	sda_release();
}
