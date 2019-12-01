#include "buzzer.h"




void Init_buzzer(void){

//		GPIO_SetDir(PORT_BUZZ, BUZZER_PIN, GPIO_DIR_OUTPUT);
//	  PIN_Configure(PORT_BUZZ,BUZZER_PIN,PIN_FUNC_0,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
//    
	
	 LPC_PINCON -> PINSEL4 = (1<<PWM_1);
	 LPC_PWM1   -> PR  = 0x00;              /* No Prescalar */
}
void buzzer_on(int frecuencia){
  

	 LPC_PWM1->MCR = (1 << 1); //Reset on PWMMR0, reset TC if it matches MR0 
	 LPC_PWM1->TCR = (1 << 0) | (1<<2); //Habilitamos eel contador y el PWM
   LPC_PWM1->PR  =   0x00;//No  preescalar, por si acaso
	 LPC_PWM1->MCR = (1<<1); //Reset cuando TC haga match con mr0
	 LPC_PWM1->MR0 = frecuencia;
	 LPC_PWM1->MR1 = 700; // Set 50% Duty Cycle for all four channels 
   LPC_PWM1->LER = (1<<0) | (1<<1) ;//cargamos los valores
	//Habillitamos las salida del pwm para p0 -p3
	 LPC_PWM1->PCR = (1<<9);//PWMENA1 1 The PWM1 output enabled.
	
}
void buzzer_off(){
  	 LPC_PWM1 -> TCR = 0x00; //Deshabilitamos el contador del PWM
		 LPC_PWM1   -> TC = 0;   // Contador a cero
}
