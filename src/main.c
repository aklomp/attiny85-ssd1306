#include <stdbool.h>
#include <avr/sleep.h>

#include "i2c.h"
#include "ssd1306.h"

int
main (void)
{
	i2c_init();
	ssd1306_init();

	// Reduce power:
	PRR = _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRADC);
	DIDR0 = _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC3D) | _BV(AIN1D) | _BV(AIN0D);

	sleep_enable();
	sleep_bod_disable();

	for (;;) {

		// Show the image:
		ssd1306_show_img();

		// Go to sleep, hopefully indefinitely:
		sleep_cpu();
	}

	return 0;
}
