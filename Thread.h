#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"        // CMSIS RTOS header file
#include "lcd.h"
#include "stdio.h"
#include "joystick.h"
#include "led_rgb.h"
#include "buzzer.h"
#include "rs232.h"

//constantes para las señales
#define REPOSO 10
#define REPROD 11
#define PROG_HORA 12
#define SIG_CANC 13
#define CANC_ANT 14
#define PAUS_REPR 15
#define CARP_ANT 16
#define SIG_CARP 17
#define SEL_RELOJ 14
#define SB_RELOJ 15
#define ACTUAL_RELOJ 16
#define REPOSO_RELOJ 17

//frecuencias buzzer
#define AGUDA 100000
#define GRAVE 80000
int Init_Thread(void);

void lcd(void const *argument);
osThreadId id_lcd;
osThreadDef (lcd, osPriorityNormal, 1, 0);

void state(void const *argument);
osThreadId id_state;
osThreadDef (state, osPriorityNormal, 1, 0);

void autom(void const *argument);
osThreadId id_autom;
osThreadDef (autom, osPriorityNormal, 1, 0);

void Init_gen(void);

typedef enum 
{ 
 REPOSO_state,
 REPROD_state ,
 PROG_HORA_state, 
// SIG_CANC_state,
// CANC_ANT_state, 
// PAUS_REPR_state, 
// CARP_ANT_state, 
// SIG_CARP_state, 
// SEL_RELOJ_state, 
// SB_RELOJ_state, 
// ACTUAL_RELOJ_state, 
// REPOSO_RELOJ_state, 
}estado_autom;
estado_autom ESTADO;




