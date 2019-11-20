#include "LPC17xx.h"
#include "RTE_Device.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "SPI_LPC17xx.h"
#include "Driver_SPI.h"

#define CTRL_PORT 0
#define PIN_RST 8  //DIP6
#define PIN_A0 6   //DIP8
#define PIN_CS 18  //DIP 11



void retardo_us(unsigned int us);
void init_lcd(void);
void wr_data(unsigned char data);
void wr_cmd(unsigned char cmd);
void copy_to_lcd(void);
void LCD_reset(void);
void reset_lcd(void);
void empezar (void);
int EscribeLetra_L1(uint8_t letra);
int EscribeLetra_L2(uint8_t letra);

