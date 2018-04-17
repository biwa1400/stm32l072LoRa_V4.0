#ifndef __LORA_H__
#define __LORA_H__
#include "stm32l0xx.h"

typedef struct
{
	uint8_t  AppEUI[8];
	uint8_t  DevEUI[8];
	uint8_t  DevNonce[2];
	uint8_t  key[16];
} LORA_SESSION;


// calling by lora.c
void LoRa_init();
void LoRa_sleep();
void LoRa_standby();
void LoRa_resetChip();

void LoRa_Set_SF (uint8_t value);
void LoRa_Set_TxPower (uint8_t value);
void LoRa_Set_Frequency (uint32_t Frequency);

void LoRa_PrintSetting();

void LoRa_Send(uint8_t* data, uint8_t size);
void RF_Receive();
void LoRa_JoinNetwork(LORA_SESSION* lora_session);

void testMic(LORA_SESSION* lora_session);
uint8_t LoRa_Tx_Done_Interrupt_Response();
uint8_t LoRa_Rx_Done_Interrupt_Response();
uint8_t LoRa_Rx_TimeOut_Interrupt_Response();

#endif