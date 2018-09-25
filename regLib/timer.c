#include "regLib.h"

static void lptimer_enable_NVIC_interrupt()
{
	NVIC_EnableIRQ(LPTIM1_IRQn); 
	NVIC_SetPriority(LPTIM1_IRQn,LPTIM1_INTERRUPT_PRIORITY); 
}

static uint8_t lptimer_init = FALSE;

void timer_lptimer_init(){
	
	if((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY)
	{
		//	printf("[INFO]Init_RTC ");
		RCC->CSR |= RCC_CSR_LSION; 
		// waiting LSI setup 
		while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY);	
	}
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_LPTIM1EN ;
	// clock select LSI 37KHz
	RCC->CCIPR &= ~RCC_CCIPR_LPTIM1SEL;
	RCC->CCIPR |= RCC_CCIPR_LPTIM1SEL_0;
	// sleep mode enable ? 
	RCC->APB1SMENR |= RCC_APB1SMENR_LPTIM1SMEN;
	// fractional 1/4
	LPTIM1->CFGR &= ~LPTIM_CFGR_PRESC;
	//LPTIM1->CFGR |= LPTIM_CFGR_PRESC_1|LPTIM_CFGR_PRESC_0;
	LPTIM1->CFGR |= LPTIM_CFGR_PRESC_1;
	
	// enable compare interrupt 
	LPTIM1->IER |= LPTIM_IER_CMPMIE; 
	lptimer_enable_NVIC_interrupt();
	
	LPTIM1->CR |= LPTIM_CR_ENABLE; /* (3) */
	lptimer_init = TRUE;
}

void timer_lptimer_Deinit(){
	
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_LPTIM1EN ;
	LPTIM1->CR &= ~LPTIM_CR_ENABLE; /* (3) */
	lptimer_init = FALSE;
}


void timer_lptimer_setNum_trige(uint16_t value){
	if(lptimer_init == FALSE)
	{
		timer_lptimer_init();
	}
	LPTIM1->CMP = value; /* (4) */
	LPTIM1->CR |= LPTIM_CR_SNGSTRT; /* start the single  */
}

void timer_lptimer_setSeconds(float seconds)
{
	// 37 kHz/32div
	uint16_t value = (37000/4)*seconds;
	timer_lptimer_setNum_trige(value);
}
