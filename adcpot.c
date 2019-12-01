#include "adcpot.h"


/*
DONEConfigure the GPIO pin for ADC function using PINSEL register.
DONEEnable the CLock to ADC module.
DONEDeselect all the channels and Power on the internal ADC module by setting ADCR.PDN bit.
DONE- IGUAL ES OTRO Select the Particular channel for A/D conversion by setting the corresponding bits in ADCR.SEL
Set the ADCR.START bit for starting the A/D conversion for selected channel.
Wait for the conversion to complete, ADGR.DONE bit will be set once conversion is over.
Read the 12-bit A/D value from ADGR.RESULT.
Use it for further processing or just display on LCD.
*/



#define PORT_ADC 1///DIP 19
#define PIN_ADC 30

void Init_adc_pot(void){
  LPC_SC->PCONP |= (1 << 12);//Alimentacion del ADC
  LPC_PINCON->PINSEL3|= 1<<28; //Canal AD0.4
  GPIO_SetDir(PORT_ADC,PIN_ADC,GPIO_DIR_INPUT);
  //PIN_Configure(PORT_ADC,PIN_ADC,PIN_FUNC_2,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
  LPC_ADC->ADCR = ((1<<21) | (1<<9));//Seleccion de reloj y modo on adc
  LPC_ADC->ADCR = (1<<4);
}

uint16_t get_vol(){
  /////////////////////
   LPC_SC->PCONP |= (1 << 12);//Alimentacion del ADC
   // LPC_PINCON->PINSEL3|= 1<<28; //Canal AD0.4
  LPC_PINCON->PINSEL3 |= (3<<29);    // P1.31 A0.5 function 11

  LPC_ADC->ADCR |= ((1<<21) | (10<<8));//Seleccion de reloj/8 y modo on adc
  LPC_ADC->ADCR |= (1<<4);
  ///////////////////////

  LPC_ADC->ADCR |= (1<<24);//start now
  //while((LPC_ADC->ADGDR & (1 << 31))>> 1 == 0);  /* wait till conversion completes */
  osDelay(5);
  LPC_ADC->ADCR &= ~(1 << 24);//PARAR LA CONVERSION
  resultado = (LPC_ADC->ADGDR>>4) & 0xfffF;///bit de resultado= adgdr 4  
  
  return resultado;
}




