#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"        // CMSIS RTOS header file
#include "lcd.h"
#include "stdio.h"

#define PORT_LED 1
#define PIN_LED1 18
#define PIN_LED2 20
#define PIN_LED3 21
#define PIN_LED4 23




///periodos leds
#define MAX_PERIOD 500
#define MIN_PERIOD 100


	struct t_tiempo
	{
		uint8_t minutos;
		uint8_t segundos;
	}tiempo;

	
void seg_callback (void const *argument);	
osTimerId tid_segundero;
osTimerDef (segundero, seg_callback);
	



/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void Led1 (void const *argument);                             // thread function
osThreadId id_Led1;                                          // thread id
osThreadDef (Led1, osPriorityNormal, 1, 0);                   // thread object

void leds(void const *argument);
osThreadId id_leds;
osThreadDef (leds, osPriorityNormal, 1, 0);

void lcd(void const *argument);
osThreadId id_lcd;
osThreadDef (lcd, osPriorityNormal, 1, 0);

void state(void const *argument);
osThreadId id_state;
osThreadDef (state, osPriorityNormal, 1, 0);



///osTrhreadDef
/// name         name of the thread function.
/// priority     initial priority of the thread function.
/// instances    number of possible thread instances.
/// stacksz      stack size (in bytes) requirements for the thread function.




int Init_Thread (void) {
	tiempo.minutos=2;
	tiempo.segundos=0;
	NVIC_EnableIRQ(EINT3_IRQn);
	GPIO_SetDir(PORT_LED, PIN_LED1, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED2, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED3, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED4, GPIO_DIR_OUTPUT);
	
	
	
	id_state = osThreadCreate (osThread(state), NULL);
  id_Led1 = osThreadCreate (osThread(Led1), NULL);
	id_leds = osThreadCreate (osThread(leds), NULL);
	id_lcd = osThreadCreate (osThread(lcd), NULL);
	tid_segundero    = osTimerCreate (osTimer(segundero), osTimerPeriodic, NULL);
	rebo             = osTimerCreate (osTimer(rebote),    osTimerOnce, (void *)0);
  if ((!id_Led1)|(!id_leds)|(!id_lcd)|(!tid_segundero)|(!id_state)) return(-1);

	
	osTimerStart(tid_segundero, 1000);
	
  return(0);
}

void seg_callback (void const *argument){

	static bool min=false;
	

	if((tiempo.segundos)>0){
	tiempo.segundos--;
		
		if(tiempo.minutos==1){
		osSignalSet(id_Led1,10);     
		}
    else{
    osSignalClear(id_Led1,10);
    }
	}
	if(tiempo.segundos==0 & tiempo.minutos!=0){
		tiempo.segundos=59;
		min=true;
	}
	if((tiempo.minutos>0) & min){
		min=false;
		tiempo.minutos--;
}
			if(tiempo.segundos==0 & tiempo.minutos==0)
				osSignalSet(id_leds,0x101);///encender leds
			}

void lcd	(void const *argument) {
	init_lcd();
	LCD_reset();

	
		while(1){
        copy_to_lcd();
        empezar();
        static uint8_t i=0;
      	static char buffer_aux1[15];
			if(tiempo.segundos>9)
			{sprintf(buffer_aux1,"Crono->0%d:%d",tiempo.minutos,tiempo.segundos);}
			else{sprintf(buffer_aux1,"Crono->0%d:0%d",tiempo.minutos,tiempo.segundos);}
			for(i=0;buffer_aux1[i] != 0x00;i++){
        EscribeLetra_L1(buffer_aux1[i]);
  }

		}

}
void Led1 (void const *argument) {
	
	while (1) {
				osSignalWait(10,osWaitForever);
      if(tiempo.segundos%4==0){
				GPIO_PinWrite (PORT_LED, PIN_LED1, 1);
				osDelay(125);//CADA 250 ms
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(125);//CADA 250 ms
      }
	}
  }

void leds(void const *argument){
	GPIO_SetDir(PORT_LED, PIN_LED1, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED2, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED3, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_LED, PIN_LED4, GPIO_DIR_OUTPUT);
	static uint16_t periodo= 250;
	bool dch= true;
  while(1){
		osSignalWait(0x101,osWaitForever);
		//osTimerStop(tid_segundero);
       if(dch){
				 dch=false;
        GPIO_PinWrite (PORT_LED, PIN_LED4, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 1);
        //osDelay(periodo);
      }
 
        else{
					dch=true;
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 1);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        osDelay(periodo);
        GPIO_PinWrite (PORT_LED, PIN_LED4, 1);
				GPIO_PinWrite (PORT_LED, PIN_LED3, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED2, 0);
				GPIO_PinWrite (PORT_LED, PIN_LED1, 0);
        //osDelay(periodo);
				}
				osSignalClear(id_leds,0x101);
      }//while
   // 	

	}
	
void state(void const *argument){
	bool pause=false;
//				LPC_GPIOINT -> IO0IntClr |= (1 << SW_RIGHT);
//				LPC_GPIOINT -> IO0IntEnR |= (1 << SW_RIGHT) ;
	while(1){
		osEvent evento;
		evento = osSignalWait(00,osWaitForever);
		osTimerStart(rebo, 200);//espera 200ms
		osSignalWait(S_ACTIVAR, osWaitForever);
		switch(evento.value.v){
			case CENTER:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER);
				if(!pause){
				osTimerStop(tid_segundero);
				pause=true;
				}
				else{
				osTimerStart(tid_segundero,1000);
				pause=false;
				}
				break;
			case BJ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_DOWN);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_DOWN);
			if(tiempo.segundos>10) tiempo.segundos -= 10;
        else if (tiempo.minutos!=0){
          tiempo.segundos = 60 -(10 -tiempo.segundos);
          tiempo.minutos --;
        }
        else{
          tiempo.minutos = tiempo.segundos = 0;
        }
			break;
			case SB:
				LPC_GPIOINT -> IO0IntClr |=  (1 << JOY_UP);
				LPC_GPIOINT -> IO0IntEnR |=  (1 << JOY_UP);	
				if(tiempo.segundos+10>60 & tiempo.minutos==0){
				tiempo.minutos++;
				tiempo.segundos=60-tiempo.segundos;	///
				}
				else{if(tiempo.minutos<=1 & tiempo.segundos+10<60)
				tiempo.segundos=tiempo.segundos+10;
				}

			break;
			case IZQ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_LF);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_LF) ;
				tiempo.minutos=2;
			  tiempo.segundos=0;	
			break;
		}
		osSignalClear(id_state,00);
		osSignalClear(id_state,S_ACTIVAR);
	}
}




