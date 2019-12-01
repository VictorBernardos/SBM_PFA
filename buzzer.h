#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"     

#define PORT_BUZZ 2
#define BUZZER_PIN 0

#define PWM_1 0

#define SBIT_PWMENA1	 9
#define SBIT_LEN0      0
#define SBIT_LEN1 		 1
#define SBIT_CNTEN     0
#define SBIT_PWMEN     2
#define SBIT_PWMMR0R 	 1


void Init_buzzer(void);
void buzzer_on(int frecuencia);
void buzzer_off(void);
