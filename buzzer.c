#include "buzzer.h"




void Init_buzzer(void){

		GPIO_SetDir(PORT_BUZZ, BUZZER_PIN, GPIO_DIR_OUTPUT);
	  PIN_Configure(PORT_BUZZ,BUZZER_PIN,PIN_FUNC_0,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
    
}
void buzzer_on(){
  
      LPC_PWM1 -> MCR |= 0x02;//Reset cuando mr0
			LPC_PWM1 -> MR0= 25000; // duracion del pulso 
			LPC_PWM1 -> MR4= 25000*0.5;// CICLO DE TRABAJO
			LPC_PWM1 -> LER=0x15;//habilitamos la carga de datos
			LPC_PWM1 -> PCR |= (1 << BUZZER_PIN); //seleccionamos los pwm y los habilitamos
		  LPC_PWM1 -> TCR |= 0x05; //Habilitamos el contador del PWM
  
}
void buzzer_off(){
  	 LPC_PWM1 -> TCR = 0x00; //Deshabilitamos el contador del PWM

}
