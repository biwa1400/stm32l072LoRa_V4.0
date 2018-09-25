#include "stdio.h"
#include "lora.h"
#include "regLib.h"
#include "main.h"
#include "ms552.h"

// RTC PRIORITY 1
// LORA 1

void EXTI0_1_IRQHandler()
{
	switch_32MHz();
	if(EXTI->PR & EXTI_PR_PR1)
	{
		EXTI->PR = EXTI_PR_PR1;
		//LoRa_Tx_Done_Interrupt_Response();
		LoRa_Rx_TimeOut_Interrupt_Response();
	}
	
}

void EXTI4_15_IRQHandler()
{
	switch_32MHz();
	if(EXTI->PR & EXTI_PR_PR4)
	{
		EXTI->PR = EXTI_PR_PR4;
		LoRa_Tx_Done_Interrupt_Response();
		LoRa_Rx_Done_Interrupt_Response();
	}	
	// ms522 state machine
	else if(EXTI->PR & EXTI_PR_PR6)
	{
		EXTI->PR = EXTI_PR_PR6;
		ms552_stateMachine();
		//printf("interrupt\n");
	}	
}

void EXTI2_3_IRQHandler()
{
	
	if(EXTI->PR & EXTI_PR_PR3)
	{
		EXTI->PR = EXTI_PR_PR3;
		main_stateMachine();
		//printf("interrupt\n");
	}	
}


void RTC_IRQHandler()
{
	// RTC wake up trigger
	switch_32MHz();
	if(EXTI->PR & EXTI_PR_PIF20)
	{
		EXTI->PR = EXTI_PR_PIF20;
					printf("RTC\n");
		if (LoRa_timer_Interrupt_Response() == 1) return;
		Interrupt_soft_EXTI_trigger(Main_State_Machine_Trigger_Interrupt_Num);
	}
}

void LPTIM1_IRQHandler()
{
	switch_32MHz();
	if((LPTIM1->ISR & LPTIM_ISR_CMPM) == LPTIM_ISR_CMPM)
	{
		LPTIM1->ICR |= LPTIM_ICR_CMPMCF;
		if (LoRa_timer_Interrupt_Response() == 1) return;
		else if (ms552_timer_Interrupt_Response() == 1) return;
	}
}





