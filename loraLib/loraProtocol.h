#ifndef __loraProtocol_H__
#define __loraProtocol_H__

#include "sx1276.h"

#define MTTYPE_JoinRequest            0x00
#define MTTYPE_JoinAccept             0x20
#define MTTYPE_UnconfirmedDataUp      0x40
#define MTTYPE_UnconfirmedDataDown    0x60
#define MTTYPE_ConfirmedDataUp        0x80
#define MTTYPE_ConfirmedDataDown      0xA0


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
	uint8_t MHDR;
	uint8_t AppNonce[3];
  uint8_t NetID[3];
	uint8_t DevAddr[4];
	uint8_t DLSettings;
	uint8_t RxDelay;
	LORA_JOIN_ACCEPT_CFList CFList;
	uint8_t mic[4];
} LORA_JOIN_ACCEPT_PHYPAYLOAD;

typedef struct
{
	uint8_t MHDR;
  uint8_t MAC_payload[256];
} LORA_PHYPAYLOAD;

typedef struct
{
	uint8_t MHDR;
	uint8_t DevAddr[4];
	uint8_t FCtrl;
	uint8_t FCnt[2];
	uint8_t FOpts_FPort_FRMPayload_MIC[256];
} LORA_DATAUP;

#ifndef LORA_DATADOWN_STANDARD_LENGTH
#define LORA_DATADOWN_STANDARD_LENGTH 8
#endif

typedef struct
{
	uint8_t MHDR;
	uint8_t DevAddr[4];
	uint8_t FCtrl;
	uint8_t FCnt[2];
	uint8_t FOpts_FPort_FRMPayload_MIC[256];
} LORA_DATADOWN;


typedef struct
{
	uint8_t isADR;
	uint8_t isADRACKReq;
	uint8_t isACK;
	uint8_t isFpending;
	uint8_t FOptsLen;
	uint8_t FOpts[16];
	uint16_t FCnt;
	uint8_t FPort;
	uint8_t FRMPayloadLength;
	uint8_t FRMPayload[256];
} U_RECEIVE_PACKET;


#endif