#include "regLib.h"
#include "stdio.h"

void IO_Interrupt_enable(IO_Interrupt_Def* io_interrupt_str)
{
	// 0x400 is offset address for each IO group
	uint32_t GPIO_group_num = ((uint32_t)(io_interrupt_str->GPIO_GROUP)-IOPPERIPH_BASE)/0x400;

// config GPIO ENABLE GPIO
	RCC->IOPENR |= 0x1U << GPIO_group_num; 

// set mode to input_mode --- 3U is 2bytes, and pin*2 because setting occupy 2 bits
	io_interrupt_str->GPIO_GROUP->MODER &= ~ (0x3U << (io_interrupt_str->pin*2));
	
// config EXIT_LINE 
	uint8_t arrayIndex = (io_interrupt_str->pin) / 4;
	uint8_t setting_pos= (io_interrupt_str->pin) % 4;
//	printf("Index: %d,Pos %d",arrayIndex,setting_pos);
	
// enable SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	//clear 
	SYSCFG->EXTICR[arrayIndex] &= (uint16_t)~(0xFU<<setting_pos);
	//setting
	SYSCFG->EXTICR[arrayIndex] |= (uint16_t)(GPIO_group_num<<setting_pos*4);
	

	EXTI->IMR |= 0x1U << io_interrupt_str->pin;
	EXTI->RTSR |= io_interrupt_str->isRisingtTrig << io_interrupt_str->pin; 
	EXTI->FTSR |= io_interrupt_str->isFallingtTrig << io_interrupt_str->pin; ; 

	uint32_t EXTI_X_IRQn=0;
	if (io_interrupt_str->pin < 2)
	{
		EXTI_X_IRQn = EXTI0_1_IRQn;
	}
	else if (io_interrupt_str->pin >=2 && io_interrupt_str->pin < 4)
	{
		EXTI_X_IRQn = EXTI2_3_IRQn;
	}
	else
	{
		EXTI_X_IRQn = EXTI4_15_IRQn;
	}
	
	NVIC_EnableIRQ(EXTI_X_IRQn); 
	NVIC_SetPriority(EXTI_X_IRQn,io_interrupt_str->interruptPriority); 

// config GPIO DISABLE GPIO
	RCC->IOPENR &= ~(0x1U << GPIO_group_num); 
}

void Interrupt_soft_EXTI_init(uint8_t trigger_Num, uint8_t priority)
{
//Configure the mask bits of the Interrupt/Event lines (EXTI_IMR, EXTI_EMR)
	EXTI->IMR |= 0x1U << trigger_Num;
/* (7) Enable Interrupt on EXTI0_1 */
/* (8) Set priority for EXTI0_1 */
	if (trigger_Num>=0 && trigger_Num<=1)
	{
		NVIC_EnableIRQ(EXTI0_1_IRQn);
		NVIC_SetPriority(EXTI0_1_IRQn,priority); 		
	}
	else if (trigger_Num>=2 && trigger_Num<=3)
	{
		NVIC_EnableIRQ(EXTI2_3_IRQn); 
		NVIC_SetPriority(EXTI2_3_IRQn,priority); 		
	}
	else
	{
		NVIC_EnableIRQ(EXTI4_15_IRQn); 
		NVIC_SetPriority(EXTI4_15_IRQn,priority); 		
	}
}

void Interrupt_soft_EXTI_trigger(uint8_t trigger_Num)
{
	EXTI->SWIER |= 0x1U << trigger_Num;
}
//Set the required bit in the software interrupt register (EXTI_SWIER).
//EXTI->SWIER



