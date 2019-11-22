#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"     

#define PORT_RGB 2

#define RED_PIN 3
#define GREEN_PIN 2
#define BLUE_PIN 1

void encender_led(int pin,int frecuencia);
void apagar_led(void);
