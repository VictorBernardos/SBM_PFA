#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"    

#define JOY_PORT       0
#define JOY_UP     23//UP
#define JOY_DOWN 17
#define JOY_RG 24
#define JOY_LF 15
#define JOY_CENTER 16


//constantes para las señales
#define IZQ 1
#define CENTER 2
#define SB 3
#define BJ 4
#define DCH 5
#define L_CENTER 6
#define S_ACTIVAR 7
#define VALOR 8


void rebote_Callback (void const *arg);
void EINT3_IRQHandler (void);
void Init_Joy(void);
