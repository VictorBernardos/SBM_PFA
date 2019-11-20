#include "led_rgb.h"

void Init_RGB(void){

	GPIO_SetDir(PORT_RGB, RED_PIN,   GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, GREEN_PIN, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, BLUE_PIN,  GPIO_DIR_OUTPUT);

}

void encender_led(int pin,int frecuencia){

}
void apagar_led(void){
}