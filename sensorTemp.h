#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Driver_I2C.h"

#define PORT_I2C 0
#define SCL_Pin 28
#define SDA_Pin 27
#define ADDR 0X90

float getData(void);

