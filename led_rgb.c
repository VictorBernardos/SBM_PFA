#include "led_rgb.h"

void Init_RGB(void){

	GPIO_SetDir(PORT_RGB, RED_PIN,   GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, GREEN_PIN, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, BLUE_PIN,  GPIO_DIR_OUTPUT);

}
//Funciona a nivel bajo
void encender_led(int pin,int frecuencia){
  GPIO_PinWrite(PORT_RGB,pin,0);
  osDelay(1/frecuencia);
  GPIO_PinWrite(PORT_RGB,pin,1);
  osDelay(1/frecuencia);
}
void apagar_led(void){
  GPIO_PinWrite(PORT_RGB,RED_PIN,1);
  GPIO_PinWrite(PORT_RGB,BLUE_PIN,1);
  GPIO_PinWrite(PORT_RGB,GREEN_PIN,1);
}
