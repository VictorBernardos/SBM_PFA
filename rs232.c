//#include "rs232.h"
#include "Driver_USART.h"
#include "cmsis_os.h"  

extern ARM_DRIVER_USART Driver_USART3;
static ARM_DRIVER_USART *USARTdrv3 = &Driver_USART3;
static int8_t Send_buf[8];

extern ARM_DRIVER_USART Driver_USART0;
static ARM_DRIVER_USART *USARTdrv0 = &Driver_USART0;
static int8_t Rec_buf[10];



void myUSART_callback(uint32_t event)
{
  uint32_t mask;
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
  if (event & mask) {
    /* Success: Wakeup Thread */
    ///osSignalSet(tid_myUART_Thread, 0x01);
  }
  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
   /// __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
    //__breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
}

void Init_RS232(void){
	USARTdrv3 ->Initialize(myUSART_callback);	
	USARTdrv3 ->PowerControl(ARM_POWER_FULL);
	USARTdrv3->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
	USARTdrv3->Control (ARM_USART_CONTROL_TX, 1);
  USARTdrv3->Control (ARM_USART_CONTROL_RX, 1);

  USARTdrv0 ->Initialize(myUSART_callback);	
	USARTdrv0 ->PowerControl(ARM_POWER_FULL);
	USARTdrv0->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
	USARTdrv0->Control (ARM_USART_CONTROL_TX, 1);
  USARTdrv0->Control (ARM_USART_CONTROL_RX, 1);
}




void sendCommand(int8_t comando, int8_t dato0, int8_t dato1)//dato apunta al buffer
{
 osDelay(20);
 Send_buf[0] = 0x7e; //starting byte
 Send_buf[1] = 0xff; //version
 Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
 Send_buf[3] = comando; //
 Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
 Send_buf[5] = dato0;//data 0
 Send_buf[6] = dato1; //data 1
 Send_buf[7] = 0xef; //ending byte
  
 USARTdrv3->Send(Send_buf,8);  


}
