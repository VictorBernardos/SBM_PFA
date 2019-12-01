#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "Driver_SPI.h"
#include "SPI_LPC17xx.h"
#include "Arial12x12.h"
#include "lcd.h"
#include "string.h"


bool reseteado=false;
char buffer[512];

uint16_t PosicionL1 =0;
uint16_t PosicionL2=128;


extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;

void retardo_us(unsigned int us){
		static uint32_t j=0;
		for (j=0; j<10*us;j++){		
	}
}

void init_lcd(){
	//Inicializar Driver SPI
	SPIdrv->Initialize(NULL);
	
	//Encender
	SPIdrv->PowerControl (ARM_POWER_FULL);
	
	//CPOL1 y CPHA1 (ARM_SPI_CPOL1_CPHA1) 
	//¿PORQUE ESA POLARIDAD Y ESA FASE?
	
	//Organización de la información MSB a LSB (ARM_SPI_MSB_LSB) ->esta puesta por defecto a LSB
	//	8 bits de datos (ARM_SPI_DATA_BITS(8)) 
	//Frecuencia del SCLK, 20MHz
	
	LPC_SC -> PCLKSEL1 |= (1 << 16);  //Selecciona 100MHz
	SPIdrv->Control (ARM_SPI_MODE_MASTER| ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8), 20000000 );
//	
//	//Tres pines del GPIO se deben configurar como salida, puestas a nivel alto 
	PIN_Configure(CTRL_PORT,PIN_RST,PIN_FUNC_0,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
	PIN_Configure(CTRL_PORT,PIN_A0,PIN_FUNC_0,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
	PIN_Configure(CTRL_PORT,PIN_CS,PIN_FUNC_2,PIN_PINMODE_PULLUP,PIN_PINMODE_NORMAL);
	
	GPIO_SetDir(CTRL_PORT, PIN_RST, GPIO_DIR_OUTPUT);
	GPIO_SetDir(CTRL_PORT, PIN_A0, GPIO_DIR_OUTPUT);
	GPIO_SetDir(CTRL_PORT, PIN_CS, GPIO_DIR_OUTPUT);
	GPIO_PinWrite(CTRL_PORT, PIN_A0, 1);
	GPIO_PinWrite(CTRL_PORT, PIN_CS, 1);
    reset_lcd();

}


void wr_data(unsigned char data)
{
  // Seleccionar CS = 0;
	GPIO_PinWrite(CTRL_PORT, PIN_CS, 0);
  // Seleccionar A0 = 1;  A0=1 los datos recibidos se guardan en la memoria del display porque esta es la información gráfica 
	//que se quiere representar.
  GPIO_PinWrite(CTRL_PORT, PIN_A0, 1);
  // Escribir un dato (data) usando la función SPIDrv->Send(…);
	//data=(uint8_t) data;
	SPIdrv ->Send(&data,sizeof(data));
  // Seleccionar CS = 1;
  GPIO_PinWrite(CTRL_PORT, PIN_CS, 1); 
}

void wr_cmd(unsigned char cmd)
{
  // Seleccionar CS = 0;
	GPIO_PinWrite(CTRL_PORT, PIN_CS, 0);
  // Seleccionar A0 = 0;
  GPIO_PinWrite(CTRL_PORT, PIN_A0, 0); 
  // Escribir un comando (cmd) usando la función SPIDrv->Send(…);
   SPIdrv ->Send(&cmd,sizeof(cmd));
  // Seleccionar CS = 1;
	GPIO_PinWrite(CTRL_PORT, PIN_CS, 1); 
}

void copy_to_lcd(void)
{
    int i;
    wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0
    wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0
    wr_cmd(0xB0);      // Página 0
    
    for(i=0;i<128;i++){
        wr_data(buffer[i]);
        }
  
     
    wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0
    wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0
    wr_cmd(0xB1);      // Página 1
     
    for(i=128;i<256;i++){
        wr_data(buffer[i]);
        }
    
    wr_cmd(0x00);       
    wr_cmd(0x10);      
    wr_cmd(0xB2);      //Página 2
    for(i=256;i<384;i++){
        wr_data(buffer[i]);
        }
    
    wr_cmd(0x00);       
    wr_cmd(0x10);       
    wr_cmd(0xB3);      // Pagina 3
     
     
    for(i=384;i<512;i++){
        wr_data(buffer[i]);
        }
}

void LCD_reset(){
	wr_cmd(0xAE); //display off
	wr_cmd(0xA2); //
	wr_cmd(0xA0); //
	wr_cmd(0xC8); //
	wr_cmd(0x22); //
	wr_cmd(0x2F); //
	wr_cmd(0x40); //
	wr_cmd(0xAF); //
	wr_cmd(0x81); //
	wr_cmd(0x17); //
	wr_cmd(0xA4); //
	wr_cmd(0xA6); //

}

int EscribeLetra_L1(uint8_t letra){
	uint8_t i, valor1, valor2;
	static uint16_t comienzo=0;
	
	comienzo = 25 *(letra - ' ');
	
	for(i=0; i < 12; i++){
		
		valor1 = Arial12x12[comienzo+i*2+1];//Cargan los valores de 2 en dos
		valor2 = Arial12x12[comienzo+i*2+2];

		buffer[i+PosicionL1] = valor1;//primera fila de pixeles
		buffer[i+PosicionL1+128] = valor2;//segunda fila de pixeles
	}
	PosicionL1 = PosicionL1 + Arial12x12[comienzo];
	
	return 0;
	}

	int EscribeLetra_L2(uint8_t letra){
	uint8_t i, valor1, valor2;
	static uint16_t comienzo=0;
		//static uint16_t PosicionL2=128;
	
	comienzo = 25 *(letra - ' ');
	
	for(i=0; i < 12; i++){
		
		valor1 = Arial12x12[comienzo+i*2+1];//Cargan los valores de 2 en dos
		valor2 = Arial12x12[comienzo+i*2+2];

		buffer[i+PosicionL2] = valor1;//primera fila de pixeles
		buffer[i+PosicionL2+128] = valor2;//segunda fila de pixeles
	}
	PosicionL2 = PosicionL2 + Arial12x12[comienzo];
	
	return 0;	
	
	}
	
void reset_lcd(void){
  GPIO_PinWrite(CTRL_PORT, PIN_RST, 0);
  retardo_us(1);
  GPIO_PinWrite(CTRL_PORT, PIN_RST, 1);
  retardo_us(1000);
}

void empezar (void){
	PosicionL1=0;
	PosicionL2=256;
}
void EscribeLinea(char buffer[]){

	for(int i=0;i<strlen(buffer);i++){
    EscribeLetra_L1(buffer[i]);
    
	}
}
void borrarBuffer(char msg[]){
	int i;
	 for(i=0 ; i < 512; i++){
      msg[i]= '\0';
  }
	 PosicionL1 = 0;
	 PosicionL2 = 0;
}
