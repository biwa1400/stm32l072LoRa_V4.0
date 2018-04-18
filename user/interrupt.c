#include "lora.h"
#include "regLib.h"

void EXTI0_1_IRQHandler()
{
	if(EXTI->PR & EXTI_PR_PR1)
	{
		EXTI->PR = EXTI_PR_PR1;
		//LoRa_Tx_Done_Interrupt_Response();
		LoRa_Rx_TimeOut_Interrupt_Response();
	}
	
}

void EXTI4_15_IRQHandler()
{
	if(EXTI->PR & EXTI_PR_PR4)
	{
		EXTI->PR = EXTI_PR_PR4;
		LoRa_Tx_Done_Interrupt_Response();
		LoRa_Rx_Done_Interrupt_Response();
	}	
}

void RTC_IRQHandler()
{
	// switch to 32MHz
	switch_32MHz();
	if(EXTI->PR & EXTI_PR_PIF20)
	{
		EXTI->PR = EXTI_PR_PIF20;
		LoRa_RTC_Interrupt_Response();
	}
	

}