#include "stdio.h"
#include "regLib.h"
#include "lora.h"
#include "loraChip.h"
#include "loraApp.h"


int main()
{
	//PA_8_clock_out_enable(PA8_CLOCK_OUT_SOURCE_SYSCLK,PA8_CLOCK_OUT_DIV16);
	
	switch_32MHz();
	
	
	static LORA_SESSION loraSession;
	LoRa_init(&loraSession);
	
	LoRa_JoinNetwork(&loraSession,FALSE,FALSE);
	delay(10);
	uint8_t sendBuf[]="dabin";
	LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
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