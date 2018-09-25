#include "regLib.h"

void stopmode_enter();
void sleepmode_enter();
void standBymode_enter();
void stopmode_enter_ISR();



void lowpower_set_always_stop (uint8_t isAlwaysStop)
{
	if(isAlwaysStop == TRUE)
	{
		SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
	}
	else
	{
			SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
	}

}

uint8_t lowpower_get_always_stop ()
{
	return (SCB->SCR &= SCB_SCR_SLEEPONEXIT_Msk)>>SCB_SCR_SLEEPONEXIT_Pos;
}

// Stop Mode
void lowpower_stopMode(float seconds)
{
	// setting sleep time
	rtc_wakeup_interrupt_setting(seconds);
	stopmode_enter();
}

void lowpower_stopMode_noTimer()
{
	// before sleep, disable HSI and PLL
	stopmode_enter();
}

// sleep mode

// Init RTC before calling this function
void lowpower_sleepMode(float second)
{
	// setting sleep time
	rtc_wakeup_interrupt_setting(second);
	sleepmode_enter();
}

void lowpower_sleepMode_noTimer()
{
	sleepmode_enter();
}

// standby mode

// Init RTC before calling this function
void lowpower_standByMode(float second)
{
	// setting sleep time
	rtc_wakeup_interrupt_setting(second);
	standBymode_enter();
}

void lowpower_standByMode_noTimer()
{
	standBymode_enter();
}


void lowpower_change_to_sleepMode()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// reset SLEEPDEEP
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;


	// reset PDDS enter standby mode when cpu deepsleep
	PWR->CR &= ~PWR_CR_PDDS;
	
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// turn off voltage regulator
	PWR->CR &= ~PWR_CR_LPSDSR;
	PWR->CR |= SLEEP_REGULATOR_STATUS << PWR_CR_LPSDSR_Pos;
	
	// ULP bit in the PWR_CR register
	PWR->CR &= ~PWR_CR_ULP;
	PWR->CR |= SLEEP_VERF_STATUS << PWR_CR_ULP_Pos;
	
	// FWU -fast_wakeup
	PWR->CR &= ~PWR_CR_FWU;
	PWR->CR |= SLEEP_WAIT_VERF_UP_STATUS << PWR_CR_FWU_Pos;

	// turn off Flash when sleep
	FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
	FLASH->ACR |= SLEEP_FLASH_STATUS << FLASH_ACR_SLEEP_PD_Pos;
}


// static function 
static void sleepmode_enter()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// reset SLEEPDEEP
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;


	// reset PDDS enter standby mode when cpu deepsleep
	PWR->CR &= ~PWR_CR_PDDS;
	
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// turn off voltage regulator
	PWR->CR &= ~PWR_CR_LPSDSR;
	PWR->CR |= SLEEP_REGULATOR_STATUS << PWR_CR_LPSDSR_Pos;
	
	// ULP bit in the PWR_CR register
	PWR->CR &= ~PWR_CR_ULP;
	PWR->CR |= SLEEP_VERF_STATUS << PWR_CR_ULP_Pos;
	
	// FWU -fast_wakeup
	PWR->CR &= ~PWR_CR_FWU;
	PWR->CR |= SLEEP_WAIT_VERF_UP_STATUS << PWR_CR_FWU_Pos;

	// turn off Flash when sleep
	FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
	FLASH->ACR |= SLEEP_FLASH_STATUS << FLASH_ACR_SLEEP_PD_Pos;
	
	// calling WFI command	
	__WFI();
}

void stopmode_enter()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// set SLEEPDEEP
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	// set PDDS enter standby mode when cpu deepsleep
	PWR->CR &= ~PWR_CR_PDDS;
	
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// turn off voltage regulator
	PWR->CR &= ~PWR_CR_LPSDSR;
	PWR->CR |= STOP_REGULATOR_STATUS << PWR_CR_LPSDSR_Pos;
	
	// ULP bit in the PWR_CR register
	PWR->CR &= ~PWR_CR_ULP;
	PWR->CR |= STOP_VERF_STATUS << PWR_CR_ULP_Pos;
	
	// FWU -fast_wakeup
	PWR->CR &= ~PWR_CR_FWU;
	PWR->CR |= STOP_WAIT_VERF_UP_STATUS << PWR_CR_FWU_Pos;

	__WFI();

}


void lowpower_change_to_stopMode()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// set SLEEPDEEP
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	// set PDDS enter standby mode when cpu deepsleep
	PWR->CR &= ~PWR_CR_PDDS;
	
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// turn off voltage regulator
	PWR->CR &= ~PWR_CR_LPSDSR;
	PWR->CR |= STOP_REGULATOR_STATUS << PWR_CR_LPSDSR_Pos;
	
	// ULP bit in the PWR_CR register
	PWR->CR &= ~PWR_CR_ULP;
	PWR->CR |= STOP_VERF_STATUS << PWR_CR_ULP_Pos;
	
	// FWU -fast_wakeup
	PWR->CR &= ~PWR_CR_FWU;
	PWR->CR |= STOP_WAIT_VERF_UP_STATUS << PWR_CR_FWU_Pos;

}



static void standBymode_enter()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// set SLEEPDEEP
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	// set PDDS enter standby mode when cpu deepsleep
	PWR->CR |= PWR_CR_PDDS;
	
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// turn off voltage regulator
	PWR->CR &= ~PWR_CR_LPSDSR;
	PWR->CR |= STANDBY_REGULATOR_STATUS << PWR_CR_LPSDSR_Pos;
	
	// ULP bit in the PWR_CR register
	PWR->CR &= ~PWR_CR_ULP;
	PWR->CR |= STANDBY_VERF_STATUS << PWR_CR_ULP_Pos;
	
	// FWU -fast_wakeup
	PWR->CR &= ~PWR_CR_FWU;
	PWR->CR |= STANDBY_WAIT_VERF_UP_STATUS << PWR_CR_FWU_Pos;

		
	__WFI();
}

static void wakeup_RTC_setting(uint32_t second)
{
	// Init RTC
	rtc_Init();
	// allow access RTC register
	rtc_allow();
	// disable wakeup timer to modify it
	RTC->CR &=~ RTC_CR_WUTE; 
	// waiting until allow to modify reload value
	while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF);
	// change wakeup time to 1Hz
	RTC->WUTR = second;
	// enable wake up interrupt
	RTC->CR |= RTC_CR_WUTIE;
	// enable wake up counter
	RTC->CR |= RTC_CR_WUTE;
	// Reset Wake up flag
	RTC->ISR &= ~RTC_ISR_WUTF;
	// disallow access
	rtc_disAllow();
	
	// configure RTC Interrupt EXTI
	SET_BIT( EXTI->IMR,EXTI_IMR_IM20);                 // enable RTC Wakeup timer interrupt line(20)
	SET_BIT( EXTI->RTSR,EXTI_RTSR_TR20);               // enable RTC Wakeup timer interrupt rising edge (20)
	NVIC_SetPriority(RTC_IRQn, 0);                     // set highest interrupt Priority for RTC EXTI (core)
	NVIC_EnableIRQ(RTC_IRQn);                          // enable  RTC EXTI (core)
}
