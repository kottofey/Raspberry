#include <bcm2835.h>
#include <stdio.h>

#define PIN RPI_V2_GPIO_P1_03


int main () {
	if (!bcm2835_init())
		return 1;

	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

	printf("Сколько раз моргнуть?\n");
	int i;
	scanf("%d", &i);


	while (i)
	{
	bcm2835_gpio_write(PIN, LOW);
	bcm2835_delay(200);
	bcm2835_gpio_write(PIN, HIGH);
	bcm2835_delay(800);
	i--;
	}

	return (bcm2835_close());

}
