#include "stdio.h"
#include "regLib.h"
#include "lora.h"
#include "loraChip.h"
#include "loraApp.h"


void recvCallBack (U_RECEIVE_PACKET* receivePacket)
{
		printf("decode payload in main: ,");
		for (int i=0; i<receivePacket->FRMPayloadLength; i++)
		{
			printf("%.2x,",receivePacket->FRMPayload[i]);
		}
}

int main()
{
	//PA_8_clock_out_enable(PA8_CLOCK_OUT_SOURCE_SYSCLK,PA8_CLOCK_OUT_DIV16);
	
	switch_32MHz();
	
	
	static LORA_SESSION loraSession;

	
	
	LoRa_init(&loraSession);
	loraSession.receiveFunc= &recvCallBack;
	LoRa_JoinNetwork(&loraSession,FALSE,FALSE);
	loraSession.ADR=TRUE;
	loraSession.ACK=TRUE;
	delay(10);
	uint8_t sendBuf[]="dabin";
	LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
	delay(10);
	LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
	delay(10);
	LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
	delay(10);
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