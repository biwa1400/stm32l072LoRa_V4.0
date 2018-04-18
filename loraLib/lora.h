#ifndef __LORA_H__
#define __LORA_H__
#include "stm32l0xx.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


typedef struct
{
	uint8_t   AppEUI[8];
	uint8_t   DevEUI[8];
	uint8_t   DevNonce[2];
	uint8_t   key[16];
	uint8_t   AppNonce[3];
	uint8_t   NetID[3];
	uint8_t   DevAddr[4];
	uint8_t   DLSettings;
	uint8_t   RxDelay;
	
	uint8_t   NwkSKey[16];
	uint8_t   AppSKey[16];
	
	uint32_t  Channel_1_Freq;
	uint32_t  Channel_2_Freq;
	uint32_t  Channel_3_Freq;
	uint32_t  Channel_4_Freq;
	uint32_t  Channel_5_Freq;
	uint32_t  Channel_6_Freq;
	uint32_t  Channel_7_Freq;
	uint32_t  Channel_8_Freq;

	
	float     receiveWaitTime;
	uint8_t   isJoinNetwork;
	uint8_t   isReceive;
	uint8_t   isSleepWaiting;
	uint8_t   isTxDone;
	uint8_t   isSendJustPubFreq;
	
	uint8_t ADR;
	uint8_t ADRACKReq;
	uint8_t ACK;
	
	uint16_t  FCnt;
} LORA_SESSION;


// calling by lora.c
void LoRa_init(LORA_SESSION* input_LORA_SESSION_ptr);


void LoRa_PrintSetting();

void LoRa_Send(uint8_t* data, uint8_t size);
void RF_Receive();


void LoRa_JoinNetwork(LORA_SESSION* lora_session, uint8_t isReJoin, uint8_t isSleepWaiting);
void LoRa_package_send(LORA_SESSION* lora_session,uint8_t FPort,uint8_t* FOpts, uint8_t FOpts_size, uint8_t* payload,uint8_t payload_size, uint8_t isConfirm);

void testMic(LORA_SESSION* lora_session);
uint8_t LoRa_Tx_Done_Interrupt_Response();
uint8_t LoRa_Rx_Done_Interrupt_Response();
uint8_t LoRa_Rx_TimeOut_Interrupt_Response();
uint8_t LoRa_RTC_Interrupt_Response();

#endif