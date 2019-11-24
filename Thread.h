#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"        // CMSIS RTOS header file
#include "lcd.h"
#include "stdio.h"
#include "joystick.h"
#include "led_rgb.h"
#include "buzzer.h"

#define VALOR 1001;

int Init_Thread(void);

void lcd(void const *argument);
osThreadId id_lcd;
osThreadDef (lcd, osPriorityNormal, 1, 0);

void state(void const *argument);
osThreadId id_state;
osThreadDef (state, osPriorityNormal, 1, 0);

void Init_gen(void);