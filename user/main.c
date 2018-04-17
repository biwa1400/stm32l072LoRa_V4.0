#include "stdio.h"
#include "regLib.h"
#include "lora.h"


int main()
{
	//PA_8_clock_out_enable(PA8_CLOCK_OUT_SOURCE_SYSCLK,PA8_CLOCK_OUT_DIV16);
	
	switch_32MHz();
  LORA_SESSION loraSession;
	LoRa_init(&loraSession);
	
	LoRa_Set_Frequency(869500000);
	LoRa_Set_TxPower (0xff);
	LoRa_Set_SF (12);
	LoRa_JoinNetwork(&loraSession);
	delay(5);
	RF_Receive();
	//LoRa_sleep();
	
	//printf("start!");
	/*
	IO_Interrupt_Def io_int_str;
	io_int_str.GPIO_GROUP=GPIOB;
	io_int_str.pin = 2;
	io_int_str.isRisingtTrig = TRUE;
	io_int_str.interruptPriority = 0;
	IO_Interrupt_enable(&io_int_str);
	RCC->IOPENR |= RCC_IOPENR_IOPBEN;
*/	
	while(1)
	{
	}

	return 0;
}