/*
Эта программка для работы с PIR датчиком
Питание и земля берется с ножек [1] и [9], а сигнал с датчика
приходит на ножку [15], которая GPIO 22

Используется библиотека bcm2835.h любезно написанная и предоставленная во всеобщее
пользование господином mikem. Скачать свежую версию можно отсюда:
http://www.airspayce.com/mikem/bcm2835/

*/


/////////////////////////Схема ножек//////////////////////////
//                                                          //
//                          лево                            //
//                     низ         верх                     //
//                           ___                            //
//          3V3 Power  [1] -| G |- [2]  5V Power            //
//       (SDA) GPIO 0  [3] -| P |- [4]  5V Power            //
//       (SCL) GPIO 1  [5] -| I |- [6]  GND                 //
//    (GPCLK0) GPIO 4  [7] -| O |- [8]  GPIO 14 (TXD)       //
//                GND  [9] -|   |- [10] GPIO 15 (RXD)       //
//            GPIO 17 [11] -| R |- [12] GPIO 18 (PCM_CLK)   //
// (PCM_DOUT) GPIO 21 [13] -| P |- [14] GND                 //
//            GPIO 22 [15] -| i |- [16] GPIO 23             //
//          3V3 Power [17] -|   |- [18] GPIO 24             //
//     (MOSI) GPIO 10 [19] -|Ver|- [20] GND                 //
//      (MISO) GPIO 9 [21] -| . |- [22] GPIO 25             //
//     (SCLK) GPIO 11 [23] -| 2 |- [24] GPIO 8 (CE0)        //
//                GND [25] -|___|- [26] GPIO 7 (CE1)        //
//                                                          //
//                    низ           верх                    //
//                          право                           //
//////////////////////////////////////////////////////////////



#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

#define PIR_SIGNAL RPI_V2_GPIO_P1_15	//ножка GPIO 22, порядковый номер P1-15



// Костыль, конечно, но возиться пока лень
// Надо будет эти функции переписать и привести к одной
// Функции выводят время в консоль и записывают в лог /home/pi/myprog/pir_log.txt
//   time_to_file_first() - когда было замечено движение
//   time_to_file_on()	- когда датчик подтвердил движение (если после паузы датчик до сих пор активен)
//   time_to_file_off()	- когда движение прекратилось (датчик вернулся в режим ожидания)

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

 printf("Start at "); mytime = time(NULL); printf(ctime(&mytime), "\n\n"); // Чисто отладочная запись в консоль, когда была запущена программа


 bcm2835_close();		// Это надо на всякий случай ибо процесс прекращается по ctrl+c и до этой функции дело не доходит
 if (!bcm2835_init())   // Инициализация GPIO и выход с кодом "1" при неудаче
  return 1;

 bcm2835_gpio_set_pud(PIR_SIGNAL, BCM2835_GPIO_PUD_DOWN);	// Подтяжка ножки к нулю
 bcm2835_gpio_fsel(PIR_SIGNAL, BCM2835_GPIO_FSEL_INPT); 	// Установка ножки для PIR на прием

//printf("%u\n", bcm2835_gpio_lev(PIR_SIGNAL)); // Вывод значения ножки

/////////Начало цикла слежения/////////

while (1) {		// Бесконечный цикл... Надо почитать про прерывания...

 while(!bcm2835_gpio_lev(PIR_SIGNAL)) 
 	{/* Ждем первого срабатывания */}	
 	
 time_to_file_first(); // см. выше
 printf("PIR activated at "); mytime = time(NULL); printf(ctime(&mytime), "\n\n");  // То было в файл, а это в консоль
 
 
// Пауза в мс. Аналог чувствительности датчика. Нужна чтобы отсечь ложные срабатывания

 bcm2835_delay(2000);
 

// Проверяем сигнал после паузы (PIR либо потухнет, либо останется активен)
// Если сигнал остался, то в консоль выводим время и ждем пока на ножке не появится ноль

 if(bcm2835_gpio_lev(PIR_SIGNAL))
	{
	 printf("Movement detected at ");
	 mytime = time(NULL); printf(ctime(&mytime), "\n"); //Вывод даты и времени
	 time_to_file_on();		// А заодно и в файл
	 while (bcm2835_gpio_lev(PIR_SIGNAL))
	 	{/* ждем пока сигнал не прекратится */}
	 printf("Movement stopped at ");
	 mytime = time(NULL); printf(ctime(&mytime), "\n"); printf("\n\n"); //Вывод даты и времени
	 time_to_file_off();	// И опять в файл
	 }
 
			
}


// До сюда обычно дело не доходит. Но пусть пока останется.

 printf("End\n");
 return (bcm2835_close());
  
}

