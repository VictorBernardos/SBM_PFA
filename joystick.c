#include "joystick.h"



extern osThreadId id_state;


void larga_Callback (void const *arg); 
osTimerId larga;                                          // timer id
osTimerDef (larga, larga_Callback);                    // define timers

bool pulsacion_larga=false;

void Init_Joy(void){
  NVIC_EnableIRQ(EINT3_IRQn);
  GPIO_SetDir(JOY_PORT,JOY_UP,  GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_DOWN,GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_LF,  GPIO_DIR_INPUT);
	GPIO_SetDir(JOY_PORT,JOY_CENTER,  GPIO_DIR_INPUT);
  GPIO_SetDir(JOY_PORT,JOY_RG,  GPIO_DIR_INPUT);
	
	PIN_Configure(JOY_PORT,JOY_UP, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_DOWN, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_LF, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(JOY_PORT,JOY_CENTER, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
  PIN_Configure(JOY_PORT,JOY_RG, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	
	LPC_GPIOINT->IO0IntEnR=((1<<JOY_CENTER)|(1<<JOY_UP)|(1<<JOY_LF)|(1<<JOY_DOWN)|(1<<JOY_RG));
  larga  = osTimerCreate (osTimer(larga),    osTimerOnce, (void *)0);

}

void EINT3_IRQHandler (void){
	///Hay que reactivar las interrupciones de cada pulsador en state_thread
	//osSignalClear(id_state, IZQ | DCH| BJ | SB | CENTER | L_CENTER);
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
      osTimerStart(larga,1000);
			osSignalSet(id_state, CENTER);
     /* if(pulsacion_larga){
        pulsacion_larga=false;
        osSignalSet(id_state, L_CENTER); 
      }
      else{
          osSignalSet(id_state, CENTER);
      }*/
		}
} 



void larga_Callback (void const *arg){
  pulsacion_larga=true;
	osSignalClear(id_state, CENTER);
	osSignalSet(id_state, L_CENTER);
}

