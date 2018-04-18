#include "lora.h"
#include <stdio.h>
#include <string.h>

#include "loraProtocol.h"
#include "LoRaMacCrypto.h"
#include "loraParameters.h"
#include "loraChip.h"
#include "sx1276.h"
#include "universal.h"
#include "regLib.h"

static LORA_SESSION* callbackSessionPtr;


//static volatile LORA_SESSION loraSession;
void LoRa_init_Parameter(LORA_SESSION* loraSession)
{
	uint8_t AppEUIValue[] = {LORA_APP_EUI_VALUE};
	memcpy((void*)(loraSession->AppEUI),AppEUIValue,8);
	
	uint8_t DevEUIValue[] = {LORA_DEV_EUI_VALUE};
	memcpy((void*)(loraSession->DevEUI),DevEUIValue,8);
	
	uint8_t LoraKeyValue[] = {LORA_KEY_VALUE};
	memcpy((void*)(loraSession->key),LoraKeyValue,16);
	
	// setting 3 default channels frequency
	loraSession->Channel_1_Freq = DEFAULT_CHANNEL1_FREQ;
	loraSession->Channel_2_Freq = DEFAULT_CHANNEL2_FREQ;
	loraSession->Channel_3_Freq = DEFAULT_CHANNEL3_FREQ;
	
	loraSession->isJoinNetwork = FALSE;
	loraSession->isReceive = FALSE;
}

void LoRa_init(LORA_SESSION* input_LORA_SESSION_ptr)
{
	LoRa_HW_init();
	LoRa_init_Parameter(input_LORA_SESSION_ptr);
	
	// default Tx Power
	LoRa_Set_TxPower (DEFAULT_TX_POWER);
	// default SF
	LoRa_Set_SF (DEFAULT_SEND_SF);
}


static void generateDevNonce(uint8_t* value)
{
	uint32_t genetrateRadonValue = RNG_getRadonValue();
	memcpy(value,&genetrateRadonValue,2);
}

void static sendTrigger (LORA_SESSION* lora_session, uint8_t* sendBuf, uint8_t sendSize)
{
	callbackSessionPtr = lora_session;
	
	// set transmit frequency
	uint16_t randomValue;
	// ge random value
	generateDevNonce((uint8_t*)&randomValue);
	if(lora_session->isSendJustPubFreq == FALSE)
	{
		randomValue = randomValue%8;
	}
	else
	{
		randomValue = randomValue%3;
	}
	// set Channel
	switch(randomValue)
	{
		case 0:
			LoRa_Set_Frequency(lora_session->Channel_1_Freq);
			break;
		case 1:
			LoRa_Set_Frequency(lora_session->Channel_2_Freq);
			break;
		case 2:
			LoRa_Set_Frequency(lora_session->Channel_3_Freq);
			break;
		case 3:
			LoRa_Set_Frequency(lora_session->Channel_4_Freq);
			break;
		case 4:
			LoRa_Set_Frequency(lora_session->Channel_5_Freq);
			break;
		case 5:
			LoRa_Set_Frequency(lora_session->Channel_6_Freq);
			break;
		case 6:
			LoRa_Set_Frequency(lora_session->Channel_7_Freq);
			break;
		case 7:
			LoRa_Set_Frequency(lora_session->Channel_8_Freq);
			break;
	}

	
	
	if (lora_session->isReceive == FALSE)
	{
			RF_Send(sendBuf, sendSize);
			return;
	}
	else
	{
		if (lora_session ->isSleepWaiting ==FALSE)
		{
			RF_Send(sendBuf, sendSize);
		}
		else
		{
			RF_Send(sendBuf, sendSize);
			while(lora_session ->isTxDone == FALSE);
			lowpower_stopMode(lora_session ->receiveWaitTime);
		}
	}
}

void LoRa_JoinNetwork(LORA_SESSION* lora_session, uint8_t isReJoin, uint8_t isSleepWaiting)
{
	if (lora_session->isJoinNetwork != FALSE && isReJoin == FALSE)
	{
		return;
	}
	
	// generate DevNonce
	generateDevNonce(lora_session->DevNonce);
	
	// build Join Packet
	LORA_JOINREQUEST joinRequest;
	joinRequest.MHDR[0]=0x00;
	memcpy(joinRequest.macPayload.AppEUI, lora_session->AppEUI,8);
	// reverse APPEUI
	bufReverse(joinRequest.macPayload.AppEUI,8);
	memcpy(joinRequest.macPayload.DevEUI, lora_session->DevEUI,8);
	// reverse DevEUI
	bufReverse(joinRequest.macPayload.DevEUI,8);
	memcpy(joinRequest.macPayload.DevNonce, lora_session->DevNonce,2);
	
	// calculate MAC
	uint8_t mic [16];
	LoRaMacJoinComputeMic(joinRequest.MHDR, sizeof(joinRequest.MHDR)+sizeof(joinRequest.macPayload),lora_session->key, mic );
	memcpy(joinRequest.mic, mic,4);
	
	uint8_t* pointer = (uint8_t*)&joinRequest;
	printf("size: %d\n",sizeof(LORA_MACPAYLOAD_JOINREQUEST));
	for(int i=0;i<sizeof(joinRequest);i++)
	{
		printf("%.2x:",pointer[i]);
	}
	
	// setting_session isSleepWaiting
	lora_session->isJoinNetwork = FALSE;
	lora_session->isSleepWaiting = isSleepWaiting;
	lora_session->isTxDone = FALSE;
	lora_session->isReceive = TRUE;
	lora_session->receiveWaitTime = JOINNETWORK_WAITING_TIME_S;
	lora_session->isSendJustPubFreq = TRUE;
	
	// send it
	sendTrigger(lora_session,pointer,sizeof(joinRequest));
}


void LoRa_package_send(LORA_SESSION* lora_session,uint8_t FPort,uint8_t* FOpts, uint8_t FOpts_size, uint8_t* payload,uint8_t payload_size, uint8_t isConfirm)
{
	if(lora_session->isJoinNetwork == FALSE)
	{
		return;
	}
	// MHDR
	LORA_DATAUP uplink_package;
	if(isConfirm != FALSE)
	{
		uplink_package.MHDR = MTTYPE_ConfirmedDataUp;
	}
	else
	{
		uplink_package.MHDR = MTTYPE_UnconfirmedDataUp;
	}
	
	// DevAddr
	memcpy(uplink_package.DevAddr,lora_session->DevAddr,4);
	// FCtrl
	uint8_t  FCtrl =0;
		// setFOptsLen
	FCtrl |= (FOpts_size&0x0f);
		// setADR 
	FCtrl |= ((lora_session->ADR&0x01)<<7);
		// setADRACKReq
	FCtrl |= ((lora_session->ADRACKReq&0x01)<<6);
		// setACK
	FCtrl |= ((lora_session->ACK&0x01)<<6);
	uplink_package.FCtrl = FCtrl;
	// FCnt
	memcpy(uplink_package.FCnt,(uint8_t*)&lora_session->FCnt,2);
	
	uint32_t pointerOffset=0;
	// FOpts
		// set FOpts
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,FOpts,FOpts_size);
	pointerOffset += FOpts_size;
	// FPort
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,&FPort,1);
	pointerOffset += 1;
	// FRMPayload
		// encode FRMPayload
	uint32_t DevAddr = *(uint32_t*)(lora_session->DevAddr);
	LoRaMacPayloadEncrypt( payload,payload_size,lora_session->AppSKey,DevAddr, 0, lora_session->FCnt, payload );
		// set FRMPayload
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,payload,payload_size);
	pointerOffset += payload_size;
	// MAC
		//Calculate MAC
	uint8_t micBuf[4];
	LoRaMacComputeMic( &uplink_package.MHDR, 8+pointerOffset,lora_session->NwkSKey, DevAddr, 0, lora_session->FCnt, (uint32_t*)micBuf );
		//set MAC
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,micBuf,4);
	pointerOffset += 4;
	
	lora_session->isSendJustPubFreq = FALSE;
	lora_session->isReceive = isConfirm;
	lora_session->receiveWaitTime = DOWNPACKET_WAITING_TIME_S;
	
	sendTrigger (lora_session, (uint8_t*)&uplink_package, 8+pointerOffset);
}

void testMic(LORA_SESSION* lora_session)
{
	uint8_t mic [16];
	uint8_t data[]={0x0, 0xb, 0x4, 0x1e, 0xf8, 0xea, 0x37, 0x75, 0x47, 0x24, 0xa0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x9c, 0x21,};
	LoRaMacJoinComputeMic(data, 19,lora_session->key, mic );
	for(int i=0;i<sizeof(mic);i++)
	{
		printf("%.2x:",mic[i]);
	}
}

	
uint8_t LoRa_Tx_Done_Interrupt_Response()
{
	// judge Interrupt Flag
	uint8_t inturruptFlags = getInterruptFlag();
	if(inturruptFlags & RFLR_IRQFLAGS_TXDONE)
	{
		printf("TX Done");
		clearInterruptFlag(RFLR_IRQFLAGS_TXDONE);
		LoRa_sleep();
		
		// set rtc timer interrupt to receive when need receive and without lowpower
		if(callbackSessionPtr ->isReceive == TRUE )
		{
			if(callbackSessionPtr->isSleepWaiting == FALSE)
			{
				rtc_wakeup_interrupt_setting(callbackSessionPtr ->receiveWaitTime);
			}
		}
		
		// change session
		callbackSessionPtr ->isTxDone = TRUE;
		
		return 1;
	}
	return 0;
}


static void packgeProcess_JoinAccept(uint8_t* packet)
{
	LORA_JOIN_ACCEPT_PHYPAYLOAD accept_phyPayload;
	memcpy(&accept_phyPayload,packet,sizeof(accept_phyPayload));
	LoRaMacJoinDecrypt( (uint8_t*)&(accept_phyPayload.AppNonce), sizeof(accept_phyPayload)-1, callbackSessionPtr->key,(uint8_t*)&(accept_phyPayload.AppNonce));

	// confirm CMAC 
	uint8_t mic[16];
		// sizeof(accept_macpayload) -4 :subtract 4 bytes of  CMAC
	LoRaMacJoinComputeMic((uint8_t*)&accept_phyPayload, sizeof(accept_phyPayload)-4, callbackSessionPtr->key, mic);
	
	if(memcmp (mic,&accept_phyPayload.mic,4) != 0)
	{
		return;		
		printf("Wrong MIC \n");
	}
	
	//set session settings
	memcpy(&(callbackSessionPtr->AppNonce),&(accept_phyPayload.AppNonce),sizeof(accept_phyPayload.AppNonce));
	memcpy(&(callbackSessionPtr->NetID),&(accept_phyPayload.NetID),sizeof(accept_phyPayload.NetID));
	memcpy(&(callbackSessionPtr->DevAddr),&(accept_phyPayload.DevAddr),sizeof(accept_phyPayload.DevAddr));
	memcpy(&(callbackSessionPtr->DLSettings),&(accept_phyPayload.DLSettings),sizeof(accept_phyPayload.DLSettings));
	memcpy(&(callbackSessionPtr->RxDelay),&(accept_phyPayload.RxDelay),sizeof(accept_phyPayload.RxDelay));
	
	uint32_t channelFreq=0;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh4,3);
	callbackSessionPtr->Channel_4_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh5,3);
	callbackSessionPtr->Channel_5_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh6,3);
	callbackSessionPtr->Channel_6_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh7,3);
	callbackSessionPtr->Channel_7_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh8,3);
	callbackSessionPtr->Channel_8_Freq = channelFreq*100;
	
	// calculate NwkSKey,AppSKey
	uint16_t devNonce = 0;
	memcpy(&devNonce,callbackSessionPtr->DevNonce,2);
	LoRaMacJoinComputeSKeys(callbackSessionPtr->key,callbackSessionPtr->AppNonce , devNonce,callbackSessionPtr->NwkSKey, callbackSessionPtr->AppSKey );

	// change session flag
	callbackSessionPtr->isJoinNetwork = TRUE;
}

static void packgeProcess_UnconfirmedDataDown(uint8_t* packet)
{
	LORA_JOIN_ACCEPT_PHYPAYLOAD accept_phyPayload;
	memcpy(&accept_phyPayload,packet,sizeof(accept_phyPayload));
	LoRaMacJoinDecrypt( (uint8_t*)&(accept_phyPayload.AppNonce), sizeof(accept_phyPayload)-1, callbackSessionPtr->key,(uint8_t*)&(accept_phyPayload.AppNonce));

	// confirm CMAC 
	uint8_t mic[16];
		// sizeof(accept_macpayload) -4 :subtract 4 bytes of  CMAC
	LoRaMacJoinComputeMic((uint8_t*)&accept_phyPayload, sizeof(accept_phyPayload)-4, callbackSessionPtr->key, mic);
	
	if(memcmp (mic,&accept_phyPayload.mic,4) != 0)
	{
		return;		
		printf("Wrong MIC \n");
	}
	
	//set session settings
	memcpy(&(callbackSessionPtr->AppNonce),&(accept_phyPayload.AppNonce),sizeof(accept_phyPayload.AppNonce));
	memcpy(&(callbackSessionPtr->NetID),&(accept_phyPayload.NetID),sizeof(accept_phyPayload.NetID));
	memcpy(&(callbackSessionPtr->DevAddr),&(accept_phyPayload.DevAddr),sizeof(accept_phyPayload.DevAddr));
	memcpy(&(callbackSessionPtr->DLSettings),&(accept_phyPayload.DLSettings),sizeof(accept_phyPayload.DLSettings));
	memcpy(&(callbackSessionPtr->RxDelay),&(accept_phyPayload.RxDelay),sizeof(accept_phyPayload.RxDelay));
	
	uint32_t channelFreq=0;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh4,3);
	callbackSessionPtr->Channel_4_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh5,3);
	callbackSessionPtr->Channel_5_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh6,3);
	callbackSessionPtr->Channel_6_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh7,3);
	callbackSessionPtr->Channel_7_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh8,3);
	callbackSessionPtr->Channel_8_Freq = channelFreq*100;
	
	// calculate NwkSKey,AppSKey
	uint16_t devNonce = 0;
	memcpy(&devNonce,callbackSessionPtr->DevNonce,2);
	LoRaMacJoinComputeSKeys(callbackSessionPtr->key,callbackSessionPtr->AppNonce , devNonce,callbackSessionPtr->NwkSKey, callbackSessionPtr->AppSKey );

	// change session flag
	callbackSessionPtr->isJoinNetwork = TRUE;
}



uint8_t LoRa_Rx_Done_Interrupt_Response()
{
	
	
	// judge Interrupt Flag
	uint8_t inturruptFlags =getInterruptFlag();
	if(inturruptFlags & RFLR_IRQFLAGS_RXDONE)
	{
		printf("RX Done");
		// clear Interrupt Flag
		clearInterruptFlag(RFLR_IRQFLAGS_RXDONE);
		
		// clear_session isReceive
		callbackSessionPtr->isReceive = FALSE;
		
		// change session
		callbackSessionPtr ->isReceive = FALSE;
		
		// Read Value From Buf
		
		uint8_t recvBuf[255];
		uint8_t rxLength = LoRaChip_readReceiveBuf(recvBuf);
		
		for (int i=0; i<rxLength; i++)
		{
			printf("%.2x,",recvBuf[i]);
		}
		
		
		// Decode Package
		LORA_PHYPAYLOAD phy_payload_str;
		memcpy(&phy_payload_str,recvBuf,rxLength);
		switch(phy_payload_str.MHDR)
		{
			case MTTYPE_JoinAccept:
				packgeProcess_JoinAccept((uint8_t*)&phy_payload_str);
			break;
				
		}
		/*
		// decode Join-Accept
		
		*/
		LoRa_sleep();
		return 1;
	}
	return 0;
}

uint8_t LoRa_Rx_TimeOut_Interrupt_Response()
{
	// judge Interrupt Flag
	uint8_t inturruptFlags = getInterruptFlag();
	if(inturruptFlags & RFLR_IRQFLAGS_RXTIMEOUT)
	{
		printf("RX TimeOut");
		clearInterruptFlag(RFLR_IRQFLAGS_RXTIMEOUT);
		LoRa_sleep();
		return 1;
	}
	return 0;
}

uint8_t LoRa_RTC_Interrupt_Response()
{
	if(callbackSessionPtr ->isReceive == TRUE)
	{
		RF_Receive();
		return 1;
	}
	return 0;
}

