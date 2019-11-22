#include "Thread.h"


#define PORT_LED 1
#define PIN_LED1 18
#define PIN_LED2 20
#define PIN_LED3 21
#define PIN_LED4 23




///periodos leds
#define MAX_PERIOD 500
#define MIN_PERIOD 100

extern double getData(void);

	struct t_tiempo
	{
		uint8_t minutos;
		uint8_t segundos;
	}tiempo;

	

	

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
///osTrhreadDef
/// name         name of the thread function.
/// priority     initial priority of the thread function.
/// instances    number of possible thread instances.
/// stacksz      stack size (in bytes) requirements for the thread function.


void rebote_Callback (void const *arg); 
osTimerId rebo;                                          // timer id
osTimerDef (rebote, rebote_Callback);                    // define timers


int Init_Thread (void) {
  GPIO_SetDir(PORT_LED,PIN_LED1,  GPIO_DIR_INPUT);
	GPIO_SetDir(PORT_LED,PIN_LED2,  GPIO_DIR_INPUT);
	NVIC_EnableIRQ(EINT3_IRQn);
	id_state = osThreadCreate (osThread(state), NULL);
	id_lcd = osThreadCreate (osThread(lcd), NULL);
	rebo             = osTimerCreate (osTimer(rebote),    osTimerOnce, (void *)0);
  if ((!id_lcd)|(!id_state)) return(-1);

	

	
  return(0);
}

//FUNCION ENCARGADA DE LLEVAR LA CUENTA DEL RELOJ
void seg_callback (void const *argument){

	static bool min=false;

			}

void lcd	(void const *argument) {
	init_lcd();
	LCD_reset();
double temp=0;
	
		while(1){
      temp=getData();
        copy_to_lcd();
        empezar();
        static uint8_t i=0;
      	static char buffer_aux1[15];
      sprintf(buffer_aux1,"Temp-> %f",temp);
      
			for(i=0;buffer_aux1[i] != 0x00;i++){
        EscribeLetra_L1(buffer_aux1[i]);
       }
		}

}


void state(void const *argument){
	bool pause=false;

	while(1){
		osEvent evento;
		evento = osSignalWait(00,osWaitForever);
    osTimerStart(rebo, 200);//espera 200ms
		osSignalWait(S_ACTIVAR, osWaitForever);
		switch(evento.value.v){
      case DCH:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_RG);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_RG) ;
        GPIO_PinWrite(PORT_LED,PIN_LED2,1);
				break;
      
        case CENTER:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER);
        GPIO_PinWrite(PORT_LED,PIN_LED1,0);

				break;
        
        case L_CENTER:
        LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
        LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER); 
        GPIO_PinWrite(PORT_LED,PIN_LED1,1);
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
    osSignalClear(id_state,IZQ|DCH|BJ|SB|CENTER|L_CENTER);
		osSignalClear(id_state,00);
		osSignalClear(id_state,S_ACTIVAR);
	}
}



void rebote_Callback (void const *arg) {
	osSignalSet(id_state,S_ACTIVAR);

//tras terminar el timer
	
}

