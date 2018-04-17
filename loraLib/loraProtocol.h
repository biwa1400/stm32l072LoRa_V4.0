#ifndef __loraProtocol_H__
#define __loraProtocol_H__

#include "sx1276.h"

typedef struct
{
	uint8_t AppEUI[8];
	uint8_t DevEUI[8];
	uint8_t DevNonce[2];
} LORA_MACPAYLOAD_JOINREQUEST;

typedef struct
{
	uint8_t MHDR[1];
  LORA_MACPAYLOAD_JOINREQUEST macPayload;
	uint8_t mic[4];
} LORA_JOINREQUEST;

typedef struct
{
	uint8_t FreqCh4[3];
	uint8_t FreqCh5[3];
	uint8_t FreqCh6[3];
	uint8_t FreqCh7[3];
	uint8_t FreqCh8[3];
	uint8_t RFU;
} LORA_JOIN_ACCEPT_CFList;

typedef struct
{
	uint8_t AppNonce[3];
  uint8_t NetID[3];
	uint8_t DevAddr[4];
	uint8_t DLSettings;
	uint8_t RxDelay;
	LORA_JOIN_ACCEPT_CFList CFList;
	uint8_t mic[4];
} LORA_JOIN_ACCEPT_PAYLOAD;

typedef struct
{
	uint8_t MHDR[1];
  LORA_JOIN_ACCEPT_PAYLOAD Payload;
} LORA_JOIN_ACCEPT;

#endif