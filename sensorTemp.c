#include "sensorTemp.h"


extern ARM_DRIVER_I2C I2C;
static ARM_DRIVER_I2C  *I2Cdrv = &I2C;

void Init_I2C(){
  //inicializar
  I2Cdrv ->Initialize(NULL);
  //encender
  I2Cdrv ->PowerControl(ARM_POWER_FULL);
  I2Cdrv ->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  
  
}
