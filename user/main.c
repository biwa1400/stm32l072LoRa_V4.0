#include "stdio.h"
#include "regLib.h"
#include "lora.h"
#include "main.h"
#include "ms552.h"

#include "loraChip.h"

#define State_LoRa_JoinNetwork 1
#define State_Sampling         2
#define State_LoRa_Send_Value  3




static uint8_t StateMachine_state = State_LoRa_JoinNetwork;

static LORA_SESSION loraSession;
static uint32_t rejoinDelay = 1;



void i2c_recvCallBack (uint8_t* receiveBuf)
{
	printf("Temp: \n");
	for (int i=0;i<3;i++){
		printf(" %.2x",receiveBuf[i]);
	}
}

void recvCallBack (U_RECEIVE_PACKET* receivePacket)
{
		printf("decode payload in main: ,");
		for (int i=0; i<receivePacket->FRMPayloadLength; i++)
		{
			printf("%.2x,",receivePacket->FRMPayload[i]);
		}
		printf("rssi: %d",loraSession.rssi);
		//Interrupt_soft_EXTI_trigger(Main_State_Machine_Trigger_Interrupt_Num);
}

void timeoutCallback ()
{
		printf("Time out callback\n");
}

void joinNetworktCallback ()
{
	 	printf("Join Network\n");
		StateMachine_state = State_LoRa_Send_Value;
		Interrupt_soft_EXTI_trigger(Main_State_Machine_Trigger_Interrupt_Num);
}


// init
void init_lora(){
		// init lora
	LoRa_init(&loraSession);
	loraSession.receiveCallback = recvCallBack;
	loraSession.receiveTimeoutCallback = timeoutCallback;
	loraSession.joinNetworkCallback =  joinNetworktCallback;
	loraSession.isSleepWaiting = TRUE;
}

void init_trigger_interrupt(){
}

void system_init(){
	switch_32MHz();
	//ms552_init();
	init_lora();
	// state machine
	Interrupt_soft_EXTI_init(Main_State_Machine_Trigger_Interrupt_Num, STATE_MACHINE_PRIORITY);
	lowpower_change_to_stopMode();
	//lowpower_set_always_stop (TRUE);
}


void main_stateMachine(){
	uint8_t sendBuf[]="dabin1";
	switch (StateMachine_state){
		case State_LoRa_JoinNetwork:
			LoRa_JoinNetwork();
			break;
		case State_Sampling:
			ms552_stateMachine();
			break;
		case State_LoRa_Send_Value:
			LoRa_package_send_simple(sendBuf,6, TRUE);
			break;
	}
}



void test1(){
		switch_32MHz();
	uint32_t i =0;
	while(TRUE){
		delay(5);
		printf("count: %d",i++);
		delay(1);
		lowpower_stopMode(5);
	}
}

void test2()
{

	switch_32MHz();
	init_lora();
	LoRa_JoinNetwork();
	while(loraSession.isJoinNetwork == FALSE);
	delay(5);
	uint8_t sendBuf[]="dabin1";
	LoRa_package_send_simple(sendBuf,7, TRUE);
	printf("finish 1!\n");
	delay(5);
		uint8_t sendBuf2[]="dabin2";
	LoRa_package_send_simple(sendBuf2,7, TRUE);
	printf("finish 2!\n");
	delay(5);
	while(1);

}




void test4(){
	//printf("start!");
	switch_32MHz();
	
	i2c1_init();
		//i2c1_transmit(0x29, 0xaa);

	uint8_t buf[3];
	i2c1_transmit_single_byte(0x77, 0x40);
	delay(0.1);
	i2c1_transmit_single_byte(0x77, 0xa2);
	delay(0.1);
	i2c1_receive(0x77,3,buf,i2c_recvCallBack);
	
	while(1);
	
}


void test5(){
	system_init();
	//stateMachine();
	while(1);
}



void test6(){
	switch_32MHz();
	rtc_allow();
	lowpower_change_to_stopMode();
	//lowpower_set_always_stop (TRUE);
	rtc_wakeup_interrupt_setting(2);
	while(1);
}

void test7(){
	switch_32MHz();
	lowpower_set_always_stop (TRUE);
	timer_lptimer_setNum_trige(0xffff);
	//rtc_wakeup_interrupt_setting(4);
	lowpower_change_to_stopMode();
	__WFI();
	while(1);
}

void test8(){
	switch_32MHz();
	init_lora();
	//lowpower_change_to_stopMode();
	//__WFI();
	
	LoRa_JoinNetwork();
	while(loraSession.isJoinNetwork == FALSE);
	delay(5);
	uint8_t sendBuf[]="dabin1";
	LoRa_package_send_simple(sendBuf,6, FALSE);
	printf("finish 1!\n");
	delay(5);
		uint8_t sendBuf2[]="dabin2";
	LoRa_package_send_simple(sendBuf,6, TRUE);
	printf("finish 2!\n");
	delay(5);
	
	while(1);
}

void test9()
{
			EXTI->FTSR = 0;
	system_init();
	Interrupt_soft_EXTI_trigger(Main_State_Machine_Trigger_Interrupt_Num);
	while(1);
}

void test10()
{
	switch_32MHz();
	EXTI->FTSR = 0;
	lowpower_change_to_stopMode();
	lowpower_set_always_stop (TRUE);
	__WFI();
	while(1);
}

void test11()
{
	switch_32MHz();
	lowpower_change_to_stopMode();
	lowpower_set_always_stop (TRUE);
	ms552_init();
	ms552_sample();
	while(1);
}

int main()
{
	test11();
	return 0;
}