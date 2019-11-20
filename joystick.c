#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"    
#include "joystick.h"

void rebote_Callback (void const *arg); 
osTimerId rebo;                                          // timer id
osTimerDef (rebote, rebote_Callback);                    // define timers

void EINT3_IRQHandler (void){
	///Hay que reactivar las interrupciones de cada pulsador en state_thread
	if (LPC_GPIOINT -> IO0IntStatR & (1 << JOY_UP)){		
			LPC_GPIOINT -> IO0IntEnR &=  ~(1 << JOY_UP) ;		  //deshabilito el que produce la interrupcion
			LPC_GPIOINT -> IO0IntClr |=  (1 << JOY_UP);
			osSignalSet(id_state, SB);
		}
		
		if(LPC_GPIOINT -> IO0IntStatR & (1 << JOY_DOWN)){
			LPC_GPIOINT -> IO0IntEnR &=  ~(1 <<JOY_DOWN) ;		//deshabilito el que produce la interrupcion
			LPC_GPIOINT -> IO0IntClr |=  (1 <<JOY_DOWN);
			osSignalSet(id_state, BJ);
		}

		if (LPC_GPIOINT -> IO0IntStatR & (1 << JOY_LF)){
			LPC_GPIOINT -> IO0IntEnR &=  ~(1 << JOY_LF) ;		//deshabilito el que produce la interrupcion
			LPC_GPIOINT -> IO0IntClr |= (1 << JOY_LF);
			osSignalSet(id_state, IZQ);
		}
		
		if (LPC_GPIOINT->IO0IntStatR & (1 << JOY_RG)){
			LPC_GPIOINT -> IO0IntEnR &=  ~(1 <<JOY_RG) ;		//deshabilito el que produce la interrupcion
			LPC_GPIOINT->IO0IntClr |=  (1 <<JOY_RG) ;
			osSignalSet(id_state, DCH);
		}
			
		
		if(LPC_GPIOINT->IO0IntStatR & (1 << JOY_CENTER)){
			LPC_GPIOINT -> IO0IntEnR &=  ~(1 <<JOY_CENTER);	//deshabilito el que produce la interrupcion
			LPC_GPIOINT->IO0IntClr |=  (1 <<JOY_CENTER); 
			osSignalSet(id_state, CENTER);
		}
} 


void Init_Joy(void){
  GPIO_SetDir(JOY_PORT,JOY_UP,  GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_DOWN,GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_LF,  GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_CENTER,  GPIO_DIR_INPUT);
	
	PIN_Configure(JOY_PORT,JOY_UP,   		PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_DOWN, 		PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_LF,      PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_CENTER,  PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	
	LPC_GPIOINT->IO0IntEnR=((1<<JOY_CENTER)|(1<<JOY_UP)|(1<<JOY_LF)|(1<<JOY_DOWN));
  rebo             = osTimerCreate (osTimer(rebote),    osTimerOnce, (void *)0);
  if (!rebo) return(-1);
  return 0;
}

void state(void const *argument){
	bool pause=false;
	while(1){
		osEvent evento;
		evento = osSignalWait(00,osWaitForever);
		osTimerStart(rebo, 200);//espera 200ms
		osSignalWait(S_ACTIVAR, osWaitForever);
		switch(evento.value.v){
			case CENTER:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER);
				
				break;
			case BJ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_DOWN);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_DOWN);
			
			break;
			case SB:
				LPC_GPIOINT -> IO0IntClr |=  (1 << JOY_UP);
				LPC_GPIOINT -> IO0IntEnR |=  (1 << JOY_UP);	
				

			break;
			case IZQ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_LF);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_LF) ;
				
			break;
		}
		osSignalClear(id_state,00);
		osSignalClear(id_state,S_ACTIVAR);
	}
}

void rebote_Callback (void const *arg) {
	osSignalSet(id_state,S_ACTIVAR);

//tras terminar el timer
	
}
