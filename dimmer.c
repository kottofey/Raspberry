#include <bcm2835.h>

#define PIN RPI_V2_GPIO_P1_03


int main()
{
    if (!bcm2835_init())
        return 1;

bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);     //Устанавливаем порт Р1_03 на вывод

unsigned int t_on, t_off; // t_on продолжительность включённого состояния= t1, а t_off- выключенного =t2

int d = 100, i, j, flag=0; // d- коэффициент заполнения в процентах, i и j, вспомогательные переменные для организации циклов, flag- если =0 светодиод затухает, если =1 разгорается

int a=10; // количество полных рабочих циклов

while (a)
{


for (j=100; j!=0; j--) // изменяем коэффициент заполнения от 100% до 0%
{
t_on=50*d; //находим t1
t_off=50*(100-d);  //находим t2
if (flag==0) d=d-1; // если светодиод затухает, уменьшаем коэффициент заполнения
if (flag==1) d=d+1; // если светодиод разгорается, увеличиваем коэффициент заполнения

for (i=10; i!=0; i--) // передаём 10 импульсов на светодиод с рассчитанными параметрами t1 и t2
          {
              bcm2835_gpio_write(PIN, LOW);
              delayMicroseconds(t_on);
              bcm2835_gpio_write(PIN, HIGH);
              delayMicroseconds(t_off);
          }
if (d==0) flag=1; // если светодиод выключен, начинаем его включать

if (d==100) flag=0; // если светодиод достиг максимума свечения, начинаем его гасить

}

a--;
}
return (!bcm2835_close ()); // Выход из программы
}
