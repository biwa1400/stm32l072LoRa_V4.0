#include "regLib.h"
#include "stdio.h"

#define ms552_STATE_SEND_TEMP_COMMAND            1
#define ms552_STATE_SEND_READ_TEMP_RAM_COMMAND   2
#define ms552_STATE_READ_TEMP                    3
#define ms552_STATE_SEND_PRESSURE_COMMAND        4
#define ms552_STATE_SEND_READ_PRESSURE_RAM_COMMAND   5
#define ms552_STATE_READ_PRESSURE                6
#define ms552_STATE_NOP                          0


#define MS552_TIMER_WAKEUP(seconds) timer_lptimer_setSeconds(seconds)
#define MS552_WAIT_SECONDS 0.1
#define MS552_State_Machine_Trigger_Interrupt_Num 6

static uint8_t ms552_StateMachine_state = ms552_STATE_SEND_TEMP_COMMAND;


void ms552_init(){
	i2c1_init();
	// init state machine interrupt
	Interrupt_soft_EXTI_init(MS552_State_Machine_Trigger_Interrupt_Num, STATE_MACHINE_PRIORITY);
		
}

void i2c_Temp_recvCallBack (uint8_t* receiveBuf)
{
	printf("Temp: \n");
	for (int i=0;i<3;i++){
		printf(" %.2x",receiveBuf[i]);
	}
	//Interrupt_soft_EXTI_trigger(MS552_State_Machine_Trigger_Interrupt_Num);
}

void i2c_Pressure_recvCallBack (uint8_t* receiveBuf)
{
	printf("Pressure: \n");
	for (int i=0;i<3;i++){
		printf(" %.2x",receiveBuf[i]);
	}
}


// mS552 sensor
static uint8_t i2cRecvBuf[3];
void ms552_stateMachine(){
		switch (ms552_StateMachine_state){
		case ms552_STATE_SEND_TEMP_COMMAND:
			i2c1_transmit_single_byte(0x77, 0x58);
			ms552_StateMachine_state = ms552_STATE_SEND_READ_TEMP_RAM_COMMAND;
			MS552_TIMER_WAKEUP(MS552_WAIT_SECONDS);
			break;
		case ms552_STATE_SEND_READ_TEMP_RAM_COMMAND:
			i2c1_transmit_single_byte(0x77, 0x00);
			ms552_StateMachine_state = ms552_STATE_READ_TEMP;
			MS552_TIMER_WAKEUP(MS552_WAIT_SECONDS);
			break;
		case ms552_STATE_READ_TEMP:
			i2c1_receive(0x77,3,i2cRecvBuf,i2c_Temp_recvCallBack);
			ms552_StateMachine_state = ms552_STATE_SEND_PRESSURE_COMMAND;
			break;
		case ms552_STATE_SEND_PRESSURE_COMMAND:
			i2c1_transmit_single_byte(0x77, 0x48);
			ms552_StateMachine_state = ms552_STATE_SEND_READ_PRESSURE_RAM_COMMAND;
			//rtc_wakeup_interrupt_setting(0.1);
			MS552_TIMER_WAKEUP(MS552_WAIT_SECONDS);
			break;
		case ms552_STATE_SEND_READ_PRESSURE_RAM_COMMAND:
			i2c1_transmit_single_byte(0x77, 0x00);
			ms552_StateMachine_state = ms552_STATE_READ_PRESSURE;
			//rtc_wakeup_interrupt_setting(0.1);
			MS552_TIMER_WAKEUP(MS552_WAIT_SECONDS);
			break;
		case ms552_STATE_READ_PRESSURE:
			i2c1_receive(0x77,3,i2cRecvBuf,i2c_Pressure_recvCallBack);
			ms552_StateMachine_state = ms552_STATE_NOP;
			break;
	}
}


void ms552_sample_temp(){
	/*
		i2c1_transmit_single_byte(0x77, 0x58);
		delay(0.1);
		i2c1_transmit_single_byte(0x77, 0x00);
		delay(0.1);
		i2c1_receive(0x77,3,i2cRecvBuf,i2c_Temp_recvCallBack);
	
	  delay(0.1);
	
		i2c1_transmit_single_byte(0x77, 0x48);
		delay(0.1);
		i2c1_transmit_single_byte(0x77, 0x00);
		delay(0.1);
		i2c1_receive(0x77,3,i2cRecvBuf,i2c_Pressure_recvCallBack);

	  delay(0.1);
	*/
	uint8_t ms552_StateMachine_state = ms552_STATE_SEND_TEMP_COMMAND;
	Interrupt_soft_EXTI_trigger(MS552_State_Machine_Trigger_Interrupt_Num);
}

uint8_t ms552_timer_Interrupt_Response()
{
	//1. wake up lora chip
	if(ms552_StateMachine_state != ms552_STATE_NOP)
	{
		// interrupt
		Interrupt_soft_EXTI_trigger(MS552_State_Machine_Trigger_Interrupt_Num);
		return 1;
	}
	return 0;
}
