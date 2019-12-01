/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/


#include "LPC17xx.h"
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "joystick.h"
#include "sensorTemp.h"
#include "lcd.h"
#include "SPI_LPC17xx.h"
#include "Driver_SPI.h"
#include "RTE_Device.h"
#include "rs232.h"

#define PORT_LED 1
#define PIN_LED1 18
#define PIN_LED3 21
#define PIN_LED4 23

//#define JOY_PORT       0
//#define JOY_UP     23//UP
extern int Init_gen(void);



/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
 
	Init_Joy();
	Init_Thread();
	Init_RGB();
	Init_I2C();
	Init_RS232();
	Init_buzzer();
  Init_adc_pot();
  osKernelStart ();                         // start thread execution 
	
	
//*****	PRUEBA*********/
/*	char buffer_aux1[100];
	static int i=0;
	uint32_t valor1=1234;
  sprintf(buffer_aux1,"Prueba valor1:%d",valor1);
  for(i=0;buffer_aux1[i]!=0x00;i++){
		EscribeLetra_L1(buffer_aux1[i]);
  
  }
  copy_to_lcd();*/
    
}
