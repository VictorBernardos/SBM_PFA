#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"     

#define PORT_BUZZ 2
#define BUZZER_PIN 0

void Init_buzzer(void);
void buzzer_on(int frecuencia);
void buzzer_off();