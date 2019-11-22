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

#define PORT_LED 1
#define PIN_LED1 18
#define PIN_LED3 21
#define PIN_LED4 23

#define JOY_PORT       0
#define JOY_UP     23//UP
extern int Init_Thread(void);



/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
  Init_Joy();
  Init_Thread();
  Init_I2C();
  osKernelStart ();                         // start thread execution 

  
    
}
