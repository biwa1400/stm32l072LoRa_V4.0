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

	//lowpower_stopMode(500);
	LoRa_init(&loraSession);	
	loraSession.receiveFunc= &recvCallBack;
	LoRa_JoinNetwork(&loraSession,FALSE,TRUE);
	loraSession.ADR=TRUE;
	loraSession.ACK=TRUE;
	while(loraSession.isBusy==TRUE);
	lowpower_stopMode(5);

	//
for (int i=5;i>0;i++)
{	
	uint8_t sendBuf[]="dabin";
	//loraSession.isSleepWaiting = FALSE;
	LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
	loraSession.FCnt +=1;
	while(loraSession.isBusy==TRUE);
	lowpower_stopMode(5);
}


/*
	switch_32MHz();
	
	
	static LORA_SESSION loraSession;

	
	
	LoRa_init(&loraSession);
	loraSession.receiveFunc= &recvCallBack;
	LoRa_JoinNetwork(&loraSession,FALSE,FALSE);
	loraSession.ADR=TRUE;
	loraSession.ACK=TRUE;
	delay(10);
	while(1)
	{
		uint8_t sendBuf[]="dabin";
		LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
		delay(10);
		LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
		delay(10);
		LoRa_package_send(&loraSession,1,0,0, sendBuf,5, TRUE);
		delay(10);
	}
*/
	return 0;
}