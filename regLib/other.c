
void PA_0_Interrupt()
{
	// IO init
	// config GPIO (PA0)
	// enable GPIO module
	RCC->IOPENR |= RCC_IOPENR_IOPAEN;
	// config GPIO mode to input (00)
	GPIOA -> MODER &= ~GPIO_MODER_MODE0_Msk;
	// config GPIO speed
	GPIOA -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0_Msk;
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEED0_0|GPIO_OSPEEDER_OSPEED0_1);
	// config GPIO pull down
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD13;
	GPIOA -> PUPDR |= GPIO_OSPEEDER_OSPEED13_1;
	// disable GPIO module
	RCC->IOPENR &= ~RCC_IOPENR_IOPAEN;

	
	// EXTI init
	//enable SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// config EXTI0 as GPIOA interrupt
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; 
	// not mask EXTI13
	EXTI->IMR |= EXTI_IMR_IM0;
	// rise edge trigger
	EXTI->RTSR |= EXTI_FTSR_TR0;
	//enable interrupt on EXTI13 in NVIC
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	// set priority for EXTI13
	NVIC_SetPriority(EXTI0_1_IRQn, 2);
}

void EXTI0_1_IRQHandler(void)
{
	printf("interrupt!!!\n  ");
	EXTI->PR |= EXTI_PR_PIF0;
}



void standBy()
{
	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// set SLEEPDEEP
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;
	// set PDDS enter standby mode when cpu deepsleep
	PWR->CR |= PWR_CR_PDDS;
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	// turn off voltage regulator
	PWR->CR |= PWR_CR_LPSDSR;
	// ULP bit in the PWR_CR register
	PWR->CR |= PWR_CR_ULP;
  // setup WKUP1
	PWR->CSR |= PWR_CSR_EWUP1;
	/*
	#ifdef DEBUG
		DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY;
	#else
		DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY;
	#endif
	*/
	
	// calling WFI command
	__WFI();
}

void stopMode()
{
	// init interrupt 

	//enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// set SLEEPDEEP
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;
	// set PDDS enter standby mode when cpu deepsleep
	PWR->CR &= ~PWR_CR_PDDS;
	// clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	// turn off voltage regulator
	PWR->CR |= PWR_CR_LPSDSR;
	// ULP bit in the PWR_CR register
	PWR->CR |= PWR_CR_ULP;

	/*
	#ifdef DEBUG
		DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY;
	#else
		DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY;
	#endif
	*/
	
	// calling WFI command
	__WFI();
}

void rtc_allow()
{
	// Enable PWR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;    
  // Allow access RTC,RTC_BACK_UP,RCC,CSR register	
	PWR->CR |= PWR_CR_DBP;
	// Disable PWR
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	
	// enable rtc in RCC_CSR
	RCC->CSR |= RCC_CSR_RTCEN;
	//write access key
	RTC->WPR = 0xCA; 
	RTC->WPR = 0x53;
}

void rtc_disAllow()
{	
	//write disallow access key
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;
	// Enable PWR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;    
  // Allow access RTC,RTC_BACK_UP,RCC,CSR register	
	PWR->CR &= ~PWR_CR_DBP;
	// Disable PWR
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	
}

void init_LSI_RTC()
{
	// Enable LSI
	RCC->CSR |= RCC_CSR_LSION; 
  // waiting LSI setup 
	while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY);	
  // set LSI as RTC clock Source
	RCC->CSR &= ~RCC_CSR_RTCSEL_Msk;
	RCC->CSR |= RCC_CSR_RTCSEL_LSI;
}

void rtc_init()
{
	// allow access RTC
	rtc_allow();
	//allow to program time,date,prescaler register register
	RTC->ISR |= RTC_ISR_INIT;        
	// wait RTC enter initializatio mode
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	// write prescaler value
	RTC->PRER = 0x002503E8; //37kHz/37(0X25)=1Khz, 1KHz/1000(0X3E8)=1Hz
	// exit initalization mode
	RTC->ISR &= ~RTC_ISR_INIT;
	// disallow RTC
	rtc_disAllow();
}

void rtc_wakeUpSetting()
{
	// allow access RTC register
	rtc_allow();
	// disable wakeup timer to modify it
	RTC->CR &=~ RTC_CR_WUTE; 
	// waiting until allow to modify reload value
	while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF);
	// change wakeup time to 1Hz
	RTC->WUTR = 0x18C0;
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


void RTC_IRQHandler(void)
{
  /* Check WUT flag */
  if((RTC->ISR & (RTC_ISR_WUTF)) == (RTC_ISR_WUTF))
  {
		rtc_allow();
    RTC->ISR &= ~RTC_ISR_WUTF; /* Reset Wake up flag */
    EXTI->PR |= EXTI_PR_PR20; /* clear exti line 20 flag */
  }
  else
  {
  }
	printf("RTC_IRQHandler");
}