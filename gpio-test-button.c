#include <bcm2835.h>

#define PIN RPI_V2_GPIO_P1_03
#define PIN_OUT RPI_V2_GPIO_P1_05
#define PIN_IN RPI_V2_GPIO_P1_07

int main () {
	if (!bcm2835_init())
		return 1;

	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_OUT, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_IN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(PIN_IN, BCM2835_GPIO_PUD_DOWN);

	bcm2835_gpio_write(PIN_OUT, HIGH);


	while (!bcm2835_gpio_lev(PIN_IN))
	{
	bcm2835_gpio_write(PIN, LOW);
	bcm2835_delay(800);
	bcm2835_gpio_write(PIN, HIGH);
	bcm2835_delay(200);
	}

	bcm2835_gpio_set_pud(PIN_IN, BCM2835_GPIO_PUD_UP);
	return (bcm2835_close());

}
