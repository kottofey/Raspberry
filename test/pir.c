#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

#define PIR_SIGNAL RPI_V2_GPIO_P1_15

int time_to_file_first(){
 
 FILE *file;

 if (!fopen("/home/pi/myprog/pir_log.txt", "a"))
	{printf("NO!\n"); return 10;}

 file = fopen("/home/pi/myprog/pir_log.txt", "a");

 time_t mytime; 
 mytime = time(NULL);
 //printf(ctime(&mytime), "\n");

 char *arr;
 arr = ctime(&mytime);
 //printf(arr);
 fprintf(file, "PIR activated ");
 fprintf(file, arr);


 fclose(file);
 return 0;

}

int time_to_file_on(){
 
 FILE *file;

 if (!fopen("/home/pi/myprog/pir_log.txt", "a"))
	{printf("NO!\n"); return 10;}

 file = fopen("/home/pi/myprog/pir_log.txt", "a");

 time_t mytime; 
 mytime = time(NULL);
 //printf(ctime(&mytime), "\n");

 char *arr;
 arr = ctime(&mytime);
 //printf(arr);
 fprintf(file, "Movement detected at ");
 fprintf(file, arr);

 fclose(file);
 return 0;

}

int time_to_file_off(){
 
 FILE *file;

 if (!fopen("/home/pi/myprog/pir_log.txt", "a"))
	{printf("NO!\n"); return 10;}

 file = fopen("/home/pi/myprog/pir_log.txt", "a");

 time_t mytime; 
 mytime = time(NULL);
 //printf(ctime(&mytime), "\n");

 char *arr;
 arr = ctime(&mytime);

 //printf(arr);
 fprintf(file, "Movement stopped at ");
 fprintf(file, arr);
 fprintf(file, "\n\n");


 fclose(file);
 return 0;

}


int main(){
time_t mytime;

 printf("Start at "); mytime = time(NULL); printf(ctime(&mytime), "\n\n");


 bcm2835_close();
 if (!bcm2835_init()) //Инициализация GPIO и выход с кодом "1" при неудаче
  return 1;

 bcm2835_gpio_set_pud(PIR_SIGNAL, BCM2835_GPIO_PUD_DOWN); //Подтяжка к нулю
 bcm2835_gpio_fsel(PIR_SIGNAL, BCM2835_GPIO_FSEL_INPT); //Установка ножки для PIR датчика в режим приема

//printf("%u\n", bcm2835_gpio_lev(PIR_SIGNAL)); // Вывод значения ножки

/////////Начало цикла слежения

while (1) {

 while(!bcm2835_gpio_lev(PIR_SIGNAL)) 
 	{/* Ждем первого срабатывания */}	
 time_to_file_first();
 printf("PIR activated at "); mytime = time(NULL); printf(ctime(&mytime), "\n\n");
 
 bcm2835_delay(2000);		// Пауза в мс - Аналог чувствительности датчика
 
 if(bcm2835_gpio_lev(PIR_SIGNAL))	// Проверяем сигнал после паузы (потухнет либо останется)
	{
	 printf("Movement detected at ");
	 mytime = time(NULL); printf(ctime(&mytime), "\n"); //Вывод даты и времени
	 time_to_file_on();
	 while (bcm2835_gpio_lev(PIR_SIGNAL))
	 	{/* ждем пока сигнал не прекратится */}
	 printf("Movement stopped at ");
	 mytime = time(NULL); printf(ctime(&mytime), "\n"); printf("\n\n"); //Вывод даты и времени
	 time_to_file_off();
	 }
 
			
}

 printf("End\n");
 return (bcm2835_close());
  
}

