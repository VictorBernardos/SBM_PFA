#include "sensorTemp.h"

extern ARM_DRIVER_I2C            Driver_I2C0;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;
 

void Init_I2C(){
  //inicializar
  I2Cdrv ->Initialize(NULL);

  uint8_t cmd[512];
  //encender
  I2Cdrv ->PowerControl(ARM_POWER_FULL);
  I2Cdrv ->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  I2Cdrv ->Control(ARM_I2C_BUS_CLEAR, 0);

}

double getData(){
  uint8_t buffer[2];
  uint8_t a[2];
  uint16_t valorHexa=0;
  uint16_t SignoTemp;
  a[0] = 0x00;
  a[1] = 0x01;
  double temperatura=0;
  I2Cdrv->MasterTransmit (ADDR, a, 2, true);
  while (I2Cdrv->GetStatus().busy);
  I2Cdrv ->MasterReceive(ADDR,buffer,1,false);
  // ultimo parametro a false ya que no tenemos ninguna operacion pendiente
  
  valorHexa = ((buffer[0]<<8)|buffer[1])>>5;//son 11bits lo que devuelve D10-D3 y luego D2-D0 D0 corresponde al bit 5
  SignoTemp = valorHexa>>10;//D10 bit de signo de la temperatura
  if(SignoTemp == 0) 
    temperatura = valorHexa*0.125;
    else
  temperatura = valorHexa*(-0.125);
  
  return temperatura;
}
