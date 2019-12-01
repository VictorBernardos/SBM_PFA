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
extern void Init_RGB(void);
extern void Init_I2C(void);
extern void Init_buzzer(void);
extern uint16_t get_vol(void);
extern void Init_adc_pot(void);

	struct t_tiempo
	{
    uint8_t horas;
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

void printlcd_Callback (void const *arg); 
osTimerId printlcd;                                          // timer id
osTimerDef (printlcd, printlcd_Callback);                    // define timers

void seg_Callback (void const *arg); 
osTimerId id_reloj;                                          // timer id
osTimerDef (reloj, seg_Callback);                    // define timers


int Init_Thread (void) {
  ESTADO=REPOSO_state;
  GPIO_SetDir(PORT_LED,PIN_LED1,  GPIO_DIR_INPUT);
	GPIO_SetDir(PORT_LED,PIN_LED2,  GPIO_DIR_INPUT);
	GPIO_SetDir(PORT_LED,PIN_LED3,  GPIO_DIR_INPUT);
	GPIO_SetDir(PORT_LED,PIN_LED4,  GPIO_DIR_INPUT);
	NVIC_EnableIRQ(EINT3_IRQn);
  
	id_state = osThreadCreate (osThread(state), NULL);
	id_lcd = osThreadCreate (osThread(lcd), NULL);
  id_autom = osThreadCreate (osThread(autom), NULL);
  

  id_reloj=osTimerCreate (osTimer(reloj),    osTimerPeriodic, (void *)0);
	rebo     = osTimerCreate (osTimer(rebote),    osTimerOnce, (void *)0);
	printlcd = osTimerCreate (osTimer(printlcd),  osTimerPeriodic, (void *)0);
  
  if ((!id_lcd)|(!id_state)|(!rebo)|(!printlcd)|(!id_reloj)|(!id_autom)) return(-1);
	osTimerStart(printlcd,100);
  return(0);
}

//void Init_gen(void){

//}


void lcd	(void const *argument) {     
  static char buffer_aux1[20];
	float temp=0;
  uint16_t vol=0;
	init_lcd();
	LCD_reset();
		while(1){
			osSignalWait(VALOR,osWaitForever);
        copy_to_lcd();
        empezar();
      vol=get_vol();
				//temp=getData();  //De prueba habria que quitar el casting
        temp=3.3;
   		sprintf(buffer_aux1,"Temperatura: %f",temp);
			EscribeLinea(buffer_aux1);
			osSignalClear(id_lcd,VALOR);
			borrarBuffer(buffer_aux1);
		}
}
void state(void const *argument){
	apagar_led();
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
				encender_led(BLUE_PIN, 4);
				buzzer_on(10000);//AGUDA
				osDelay(500);
				buzzer_off();
				break;
      
        case CENTER:
					apagar_led();
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER);
        GPIO_PinWrite(PORT_LED,PIN_LED3,1); //no funciona
				buzzer_on(8000);//GRAVE
				osDelay(250);
				buzzer_off();
				break;
        
        case L_CENTER:
        LPC_GPIOINT -> IO0IntClr |= (1 << JOY_CENTER);
        LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_CENTER); 
        //GPIO_PinWrite(PORT_LED,PIN_LED1,1); no funciona
				
        break;
			case BJ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_DOWN);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_DOWN);
				encender_led(RED_PIN, 4);
			break;
			case SB:
				LPC_GPIOINT -> IO0IntClr |=  (1 << JOY_UP);
				LPC_GPIOINT -> IO0IntEnR |=  (1 << JOY_UP);	
			GPIO_PinWrite(PORT_LED,PIN_LED1,0);
			GPIO_PinWrite(PORT_LED,PIN_LED2,0);
			GPIO_PinWrite(PORT_LED,PIN_LED3,0);
			GPIO_PinWrite(PORT_LED,PIN_LED4,0);
				encender_led(GREEN_PIN, 4);
				

			break;
			case IZQ:
				LPC_GPIOINT -> IO0IntClr |= (1 << JOY_LF);
				LPC_GPIOINT -> IO0IntEnR |= (1 << JOY_LF) ;
				
			break;
      
		}
    
		osSignalClear(id_state,00);
		osSignalClear(id_state,S_ACTIVAR);
		buzzer_off();
		//osThreadYield();

	}
}


void autom(void const *argument){
  osEvent evento;
  uint8_t carpeta=0;  //empezamos en el 00, 01, 02
  uint8_t vsel_var_reloj=0; //0 para hora, 1 para min, 2 para seg
  bool esta_pausado=false;
  while(1){
		
    switch(ESTADO){
      case REPOSO_state:
        
        if(L_CENTER)
          ESTADO= REPROD_state;
        break;
      case REPROD_state:
        evento = osSignalWait(00,osWaitForever);
        osTimerStart(rebo, 200);//espera 200ms
		
        osSignalWait(S_ACTIVAR, osWaitForever);
        switch(evento.value.v){
            case(L_CENTER):
              ESTADO= PROG_HORA_state;
            break;
            case IZQ:
               sendCommand(CMD_PREV_SONG, 0, 0); //cancion anterior
            break;
            case DCH:
               sendCommand(CMD_NEXT_SONG, 0, 0); //cancion anterior
            break;
            case SB:
               sendCommand(CMD_PREV_SONG,carpeta, 0); //cancion anterior data0=01, data1=00
               carpeta++;
               if(carpeta==2)
                 carpeta=0;
            break;
            case BJ:
               sendCommand(CMD_PREV_SONG,carpeta, 0); //cancion anterior data0=01, data1=00
               carpeta--;
               if(carpeta==0)
                 carpeta=2;
            break;
            case CENTER:
              if (esta_pausado)
                sendCommand(CMD_PLAY,0, 0);
              else
                sendCommand(CMD_PAUSE,0, 0);
              esta_pausado=  ~esta_pausado;
            break;
            }
         break;
      case PROG_HORA_state:
        ////////////////////////////////////////////////HAY QUE PARAR EL RELOJ//////////
        evento = osSignalWait(00,osWaitForever);
        osTimerStart(rebo, 200);//espera 200ms
		
        osSignalWait(S_ACTIVAR, osWaitForever);
        switch(evento.value.v){
          case(L_CENTER):
            ESTADO= REPOSO_state;
          break;
          case IZQ:
            if(vsel_var_reloj==0)
                 vsel_var_reloj=2;
            vsel_var_reloj--;
          break;
          case DCH:
            if(vsel_var_reloj==2)
                 vsel_var_reloj=0;
            vsel_var_reloj--;
          break;
          case SB:
            if(vsel_var_reloj==0)
              tiempo.horas++;
            if(vsel_var_reloj==1)
              tiempo.minutos++;
            if(vsel_var_reloj==2)
              tiempo.segundos++;
          break;
          case BJ:
            if(vsel_var_reloj==0)
              tiempo.horas--;
            if(vsel_var_reloj==1)
              tiempo.minutos--;
            if(vsel_var_reloj==2)
              tiempo.segundos--;
          break;
          case CENTER:
            ///////////////////////////////////SET del tiempo////////
          break;
        }
        break;   
    }//SWITCH
    //osSignalClear(id_autom,00);
  }//while
  
}

//FUNCION ENCARGADA DE LLEVAR LA CUENTA DEL RELOJ
void seg_Callback (void const *argument){
  tiempo.segundos ++; 
		if (tiempo.segundos == 60){
			tiempo.segundos = 0;
			tiempo.minutos++;
		}
		if(tiempo.minutos == 60){
			tiempo.minutos = 0;
			tiempo.horas++;
		}
		if (tiempo.horas == 24){
			tiempo.horas = 0;
		}
}

void rebote_Callback (void const *arg) {
	osSignalSet(id_state,S_ACTIVAR);

//tras terminar el timer
	
}
void printlcd_Callback (void const *arg){
	osSignalSet(id_lcd,VALOR);
}
