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

#define LORA_TIMER_WAKEUP(seconds) timer_lptimer_setSeconds(seconds)

static LORA_SESSION* lora_session_ptr;

static void cancel_enterStopMode();

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
	// set session PTR
	lora_session_ptr = input_LORA_SESSION_ptr;
	
}


static void generateDevNonce(uint8_t* value)
{
	uint32_t genetrateRadonValue = RNG_getRadonValue();
	memcpy(value,&genetrateRadonValue,2);
}

void static sendTrigger (uint8_t* sendBuf, uint8_t sendSize)
{
	// set transmit frequency
	uint16_t randomValue;
	// ge random value
	generateDevNonce((uint8_t*)&randomValue);
	if(lora_session_ptr->isSendJustPubFreq == FALSE)
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
			LoRa_Set_Frequency(lora_session_ptr->Channel_1_Freq);
			break;
		case 1:
			LoRa_Set_Frequency(lora_session_ptr->Channel_2_Freq);
			break;
		case 2:
			LoRa_Set_Frequency(lora_session_ptr->Channel_3_Freq);
			break;
		case 3:
			LoRa_Set_Frequency(lora_session_ptr->Channel_4_Freq);
			break;
		case 4:
			LoRa_Set_Frequency(lora_session_ptr->Channel_5_Freq);
			break;
		case 5:
			LoRa_Set_Frequency(lora_session_ptr->Channel_6_Freq);
			break;
		case 6:
			LoRa_Set_Frequency(lora_session_ptr->Channel_7_Freq);
			break;
		case 7:
			LoRa_Set_Frequency(lora_session_ptr->Channel_8_Freq);
			break;
	}

	LoRa_wakeUp();
	RF_Send(sendBuf, sendSize);
}


void LoRa_JoinNetwork()
{

	// generate DevNonce
	generateDevNonce(lora_session_ptr->DevNonce);
	
	// build Join Packet
	LORA_JOINREQUEST joinRequest;
	joinRequest.MHDR[0]=0x00;
	memcpy(joinRequest.macPayload.AppEUI, lora_session_ptr->AppEUI,8);
	// reverse APPEUI
	bufReverse(joinRequest.macPayload.AppEUI,8);
	memcpy(joinRequest.macPayload.DevEUI, lora_session_ptr->DevEUI,8);
	// reverse DevEUI
	bufReverse(joinRequest.macPayload.DevEUI,8);
	memcpy(joinRequest.macPayload.DevNonce, lora_session_ptr->DevNonce,2);
	
	// calculate MAC
	uint8_t mic [16];
	LoRaMacJoinComputeMic(joinRequest.MHDR, sizeof(joinRequest.MHDR)+sizeof(joinRequest.macPayload),lora_session_ptr->key, mic );
	memcpy(joinRequest.mic, mic,4);
	
	uint8_t* pointer = (uint8_t*)&joinRequest;
	
	// setting flags
	lora_session_ptr ->isBusy = TRUE;
	lora_session_ptr->isJoinNetwork = FALSE;;
	lora_session_ptr->isReceive = TRUE;
	lora_session_ptr->receiveWaitTime = JOINNETWORK_WAITING_TIME_S;
	lora_session_ptr->isSendJustPubFreq = TRUE;
	
	// send it
	sendTrigger(pointer,sizeof(joinRequest));
}

void LoRa_package_send_simple ( uint8_t* payload,uint8_t payload_size, uint8_t isReceive)
{
	 LoRa_package_send(1,0,0, payload,payload_size, isReceive);
}


void LoRa_package_send(uint8_t FPort,uint8_t* FOpts, uint8_t FOpts_size, uint8_t* payload,uint8_t payload_size, uint8_t isConfirm)
{
	if(lora_session_ptr->isJoinNetwork == FALSE)
	{
		printf("Not join network\n");
		return;
	}
	// MHDR
	LORA_DATAUP uplink_package;
	if(isConfirm != FALSE)
	{
		uplink_package.MHDR = MTTYPE_ConfirmedDataUp;
		lora_session_ptr->ACK = TRUE;
	}
	else
	{
		uplink_package.MHDR = MTTYPE_UnconfirmedDataUp;
		lora_session_ptr->ACK = FALSE;
	}
	
	// DevAddr
	memcpy(uplink_package.DevAddr,lora_session_ptr->DevAddr,4);
	// FCtrl
	uint8_t  FCtrl =0;
		// setFOptsLen
	FCtrl |= (FOpts_size&0x0f);
		// setADR 
	FCtrl |= ((lora_session_ptr->ADR&0x01)<<7);
		// setADRACKReq
	FCtrl |= ((lora_session_ptr->ADRACKReq&0x01)<<6);
		// setACK
	FCtrl |= ((lora_session_ptr->ACK&0x01)<<5);
	uplink_package.FCtrl = FCtrl;
	// FCnt
	lora_session_ptr->FCnt += 1;
	memcpy(uplink_package.FCnt,(uint8_t*)&lora_session_ptr->FCnt,2);
	
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
	uint32_t DevAddr = *(uint32_t*)(lora_session_ptr->DevAddr);
	// copy payload
	
	uint8_t payload_encrypt[payload_size];
	
	LoRaMacPayloadEncrypt( payload,payload_size,lora_session_ptr->AppSKey,DevAddr, 0, lora_session_ptr->FCnt, payload_encrypt );
		// set FRMPayload
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,payload_encrypt,payload_size);
	pointerOffset += payload_size;
	// MAC
		//Calculate MAC
	uint8_t micBuf[4];
	LoRaMacComputeMic( &uplink_package.MHDR, 8+pointerOffset,lora_session_ptr->NwkSKey, DevAddr, 0, lora_session_ptr->FCnt, (uint32_t*)micBuf );
		//set MAC
	memcpy(uplink_package.FOpts_FPort_FRMPayload_MIC+pointerOffset,micBuf,4);
	pointerOffset += 4;
	
	lora_session_ptr->isSendJustPubFreq = FALSE;
	lora_session_ptr->isReceive = isConfirm;
	lora_session_ptr->receiveWaitTime = DOWNPACKET_WAITING_TIME_S;
	
	sendTrigger ((uint8_t*)&uplink_package, 8+pointerOffset);
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

	




static void packgeProcess_JoinAccept(uint8_t* packet)
{
	LORA_JOIN_ACCEPT_PHYPAYLOAD accept_phyPayload;
	memcpy(&accept_phyPayload,packet,sizeof(accept_phyPayload));
	LoRaMacJoinDecrypt( (uint8_t*)&(accept_phyPayload.AppNonce), sizeof(accept_phyPayload)-1, lora_session_ptr->key,(uint8_t*)&(accept_phyPayload.AppNonce));

	// confirm CMAC 
	uint8_t mic[16];
		// sizeof(accept_macpayload) -4 :subtract 4 bytes of  CMAC
	LoRaMacJoinComputeMic((uint8_t*)&accept_phyPayload, sizeof(accept_phyPayload)-4, lora_session_ptr->key, mic);
	
	if(memcmp (mic,&accept_phyPayload.mic,4) != 0)
	{
		return;		
		printf("Wrong MIC \n");
	}
	
	//set session settings
	memcpy(&(lora_session_ptr->AppNonce),&(accept_phyPayload.AppNonce),sizeof(accept_phyPayload.AppNonce));
	memcpy(&(lora_session_ptr->NetID),&(accept_phyPayload.NetID),sizeof(accept_phyPayload.NetID));
	memcpy(&(lora_session_ptr->DevAddr),&(accept_phyPayload.DevAddr),sizeof(accept_phyPayload.DevAddr));
	memcpy(&(lora_session_ptr->DLSettings),&(accept_phyPayload.DLSettings),sizeof(accept_phyPayload.DLSettings));
	memcpy(&(lora_session_ptr->RxDelay),&(accept_phyPayload.RxDelay),sizeof(accept_phyPayload.RxDelay));
	
	uint32_t channelFreq=0;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh4,3);
	lora_session_ptr->Channel_4_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh5,3);
	lora_session_ptr->Channel_5_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh6,3);
	lora_session_ptr->Channel_6_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh7,3);
	lora_session_ptr->Channel_7_Freq = channelFreq*100;
	memcpy(&channelFreq,&accept_phyPayload.CFList.FreqCh8,3);
	lora_session_ptr->Channel_8_Freq = channelFreq*100;
	
	// calculate NwkSKey,AppSKey
	uint16_t devNonce = 0;
	memcpy(&devNonce,lora_session_ptr->DevNonce,2);
	LoRaMacJoinComputeSKeys(lora_session_ptr->key,lora_session_ptr->AppNonce , devNonce,lora_session_ptr->NwkSKey, lora_session_ptr->AppSKey );

	// change session flag
	lora_session_ptr->FCnt = 0;
	// setting flags
	lora_session_ptr->isJoinNetwork = TRUE;
	
	if(lora_session_ptr->joinNetworkCallback != NULL)
	{
		lora_session_ptr->joinNetworkCallback();
	}
}

static void packgeProcess_ConfirmedDataDown(uint8_t* packet,uint8_t rxLength)
{
	printf("Package:\n");
		for (int i=0; i<rxLength; i++)
		{
			printf("%.2x,",packet[i]);
		}
	
		// LoRa Protocol Struct
		LORA_DATADOWN downlink_package;
		// copy valye to LORA_DATADOWN
		memcpy(&downlink_package, packet, rxLength);
		// User Receive Data Struct
		U_RECEIVE_PACKET userReceivePacket;

		// Decode FCtrl
			// get ADR
		userReceivePacket.isADR = (downlink_package.FCtrl&0x80)>>7;
		// get ADRACKReq
		userReceivePacket.isADRACKReq = (downlink_package.FCtrl&0x40)>>6;
		// get ACK
		userReceivePacket.isACK = (downlink_package.FCtrl&0x20)>>5;
		// get FPending
	  userReceivePacket.isFpending = (downlink_package.FCtrl&0x10)>>4;	
		// get FOptsLen
    userReceivePacket.FOptsLen = downlink_package.FCtrl&0x0f;	
		
		// Decode FCnt
		memcpy(&userReceivePacket.FCnt,downlink_package.FCnt,2);

		// verify MIC
			// get packet_mic	
		uint32_t package_mic;
		memcpy(&package_mic,packet+rxLength-4,4);
			//calculate mic
		uint32_t calculate_mic = 0;

		LoRaMacComputeMic(packet, rxLength-4, lora_session_ptr->NwkSKey,*(uint32_t*)(lora_session_ptr->DevAddr), 1, *(uint16_t*)(downlink_package.FCnt), &calculate_mic );
		/*
		printf("CalculateMac: ,");
		for (int i=0; i<4; i++)
		{
			printf("%.2x,",((uint8_t*)(&calculate_mic))[i]);
		}
		*/

		if(package_mic != calculate_mic)
		{
			return;
		}

	
		
		uint8_t FOpts_FPort_FRMPayload_MIC_ptr = 0;
		// get FOpts
		memcpy(&userReceivePacket.FOpts,downlink_package.FOpts_FPort_FRMPayload_MIC+FOpts_FPort_FRMPayload_MIC_ptr,userReceivePacket.FOptsLen);
		FOpts_FPort_FRMPayload_MIC_ptr += userReceivePacket.FOptsLen;
		// get FPort
	  memcpy(&userReceivePacket.FPort,downlink_package.FOpts_FPort_FRMPayload_MIC+FOpts_FPort_FRMPayload_MIC_ptr,1);
		FOpts_FPort_FRMPayload_MIC_ptr += 1;
		// get Payload & payloadLength
		userReceivePacket.FRMPayloadLength = rxLength - LORA_DATADOWN_STANDARD_LENGTH - FOpts_FPort_FRMPayload_MIC_ptr-4;
		memcpy(&userReceivePacket.FRMPayload,downlink_package.FOpts_FPort_FRMPayload_MIC+FOpts_FPort_FRMPayload_MIC_ptr,userReceivePacket.FRMPayloadLength);	
		
		// decode Payload
		uint8_t *key = lora_session_ptr->AppSKey;
		if(userReceivePacket.FPort == 0)
		{
			key = lora_session_ptr->NwkSKey;
		}

		
		LoRaMacPayloadDecrypt( userReceivePacket.FRMPayload, userReceivePacket.FRMPayloadLength, key, *(uint32_t*)(lora_session_ptr->DevAddr), 1, *(uint16_t*)downlink_package.FCnt, userReceivePacket.FRMPayload );
		// call callback function
		if(lora_session_ptr->receiveCallback != NULL)
		{
			lora_session_ptr->receiveCallback(&userReceivePacket);
		}
}




static void cancel_enterStopMode()
{
	if(lowpower_get_always_stop() == FALSE){
		lowpower_set_always_stop (FALSE);
	}
}



/*******LoRa Event**********/

uint8_t LoRa_Tx_Done_Interrupt_Response()
{
	// judge Interrupt Flag
	uint8_t inturruptFlags = getInterruptFlag();
	if(inturruptFlags & RFLR_IRQFLAGS_TXDONE)
	{
		
		clearInterruptFlag(RFLR_IRQFLAGS_TXDONE);
		LoRa_sleep();		
		
		// set rtc timer interrupt to receive when need receive and without lowpower
		if(lora_session_ptr ->isReceive == TRUE )
		{
				LORA_TIMER_WAKEUP(lora_session_ptr ->receiveWaitTime);
		}
		else{
					lora_session_ptr ->isBusy = FALSE;
		}
		return 1;
	}
	return 0;
}

uint8_t LoRa_timer_Interrupt_Response()
{
	//1. wake up lora chip
	if(lora_session_ptr ->isReceive == TRUE)
	{
		// start Rx
	  LoRa_wakeUp();
		RF_Receive();
		return 1;
	}
	return 0;
}



uint8_t LoRa_Rx_Done_Interrupt_Response()
{
	
	// judge Interrupt Flag
	uint8_t inturruptFlags =getInterruptFlag();
	if(inturruptFlags & RFLR_IRQFLAGS_RXDONE)
	{
		// clear Interrupt Flag
		clearInterruptFlag(RFLR_IRQFLAGS_RXDONE);
		
		// clear_session isReceive
		lora_session_ptr->isReceive = FALSE;
		
		// get RSSI
		lora_session_ptr->rssi = getRssi();
		
		// Read Value From Buf
		
		uint8_t recvBuf[255];
		uint8_t rxLength = LoRaChip_readReceiveBuf(recvBuf);
		
		// Decode Package
		LORA_PHYPAYLOAD phy_payload_str;
		memcpy(&phy_payload_str,recvBuf,rxLength);
		switch(phy_payload_str.MHDR)
		{
			case MTTYPE_JoinAccept:
				packgeProcess_JoinAccept((uint8_t*)&phy_payload_str);
				break;
			case MTTYPE_ConfirmedDataDown:
				packgeProcess_ConfirmedDataDown((uint8_t*)&phy_payload_str, rxLength);
				break;
				
		}
		// turn off lora chip
		LoRa_sleep();
		
		// setting flags
		lora_session_ptr ->isBusy = FALSE;
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
		clearInterruptFlag(RFLR_IRQFLAGS_RXTIMEOUT);
		lora_session_ptr->isBusy = FALSE;
		// clear_session isReceive
		lora_session_ptr->isReceive = FALSE;
		
		//turn off lora chip
		LoRa_sleep();
		
		// setting flags
		lora_session_ptr ->isBusy = FALSE;
		// call callback function
		if(lora_session_ptr->receiveTimeoutCallback != NULL)
		{
			lora_session_ptr->receiveTimeoutCallback();
		}
		return 1;
	}
	return 0;
}



