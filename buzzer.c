#include "buzzer.h"



void Init_buzzer(void){

		GPIO_SetDir(PORT_BUZZ, BUZZER_PIN, GPIO_DIR_OUTPUT);
	  PIN_Configure(PORT_BUZZ,BUZZER_PIN,PIN_FUNC_0,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
	
}
void buzzer_on(int frecuencia){
	
}
void buzzer_off(){
}