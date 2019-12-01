#include "sensorTemp.h"

extern ARM_DRIVER_I2C            Driver_I2C0;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;
 
 extern void retardo_us(int);
void Init_I2C(){
  //inicializar
  I2Cdrv ->Initialize(NULL);
  //encender
  I2Cdrv ->PowerControl(ARM_POWER_FULL);
  I2Cdrv ->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  I2Cdrv ->Control(ARM_I2C_BUS_CLEAR, 0);

}

float getData(){
  uint8_t buffer[2];
  uint8_t a= 0x00;
  uint16_t valorHexa;
  uint16_t SignoTemp;
  float temperatura;
  retardo_us(1000);
  I2Cdrv->MasterTransmit (ADDR, &a, 1, true);
  while (I2Cdrv->GetStatus().busy);
  I2Cdrv ->MasterReceive(ADDR,buffer,2,false);
  // ultimo parametro a false ya que no tenemos ninguna operacion pendiente
  
  valorHexa = ((buffer[0]<<8)|buffer[1])>>5;//son 11bits lo que devuelve D10-D3 y luego D2-D0 D0 corresponde al bit 5
  SignoTemp = valorHexa>>10;//D10 bit de signo de la temperatura
  if(SignoTemp == 0) 
    temperatura = valorHexa*0.125;
    else
  temperatura = valorHexa*(-0.125);
  
  return temperatura;
}
