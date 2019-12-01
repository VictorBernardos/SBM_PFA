#include "LPC17xx.h"
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"



uint16_t resultado;
void Init_adc_pot(void);
uint16_t get_vol(void);