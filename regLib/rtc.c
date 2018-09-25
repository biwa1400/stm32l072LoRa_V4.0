#include "regLib.h"
#include "stdio.h"



static uint8_t rtcRangeMode = RTC_RANGE_MODE_HZ;

void rtc_allow()
{
	// Enable PWR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;    
  // Allow access RTC,RTC_BACK_UP,RCC,CSR register	
	PWR->CR |= PWR_CR_DBP;
	// Disable PWR
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	
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

static volatile uint8_t isRTC_Init=0;

void rtc_Init()
{
	if(isRTC_Init) return;
	// 1. LSI setup and set as clock source
	// Enable LSI
//	printf("[INFO]Init_RTC ");
	RCC->CSR |= RCC_CSR_LSION; 
  // waiting LSI setup 
	while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY);	


	// 2. setting rtc register 
	// allow access RTC
	rtc_allow();
	
	// reset RTC
	RCC->CSR |= RCC_CSR_RTCRST;
	RCC->CSR &= ~RCC_CSR_RTCRST;
	// set LSI as RTC clock Source
	RCC->CSR |= RCC_CSR_RTCSEL_LSI;
	
	// enable rtc in RCC_CSR
	RCC->CSR |= RCC_CSR_RTCEN;
	
	// allow again after reset RTC
	rtc_allow();
	//allow to program time,date,prescaler register register
	RTC->ISR |= RTC_ISR_INIT;        
	// wait RTC enter initializatio mode
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	// write prescaler value
	RTC->PRER = 0x002503E8; //37kHz/37(0X25)=1Khz, 1KHz/1000(0X3E8)=1Hz
	//RTC->PRER = 0x0025; //37kHz/37(0X25)=1Hz
	// disable wakeup counter
	RTC->CR &= ~RTC_CR_WUTE;
	// change wakeup clock selection
	
	// wakeup use 1Hz frequency
	RTC->CR &= ~RTC_CR_WUCKSEL;
	RTC->CR |= (RTC_CR_WUCKSEL_2);
	
	while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF);
	// exit initalization mode
	RTC->ISR &= ~RTC_ISR_INIT;
	// disallow RTC
	rtc_disAllow();
	isRTC_Init = 1;
}


static void rtc_chagePrescaler(uint8_t mode)
{
	if (mode == rtcRangeMode)
	{
		return;
	}
	// allow access RTC
	rtc_allow();
	//allow to program time,date,prescaler register register
	RTC->ISR |= RTC_ISR_INIT;        
	// wait RTC enter initializatio mode
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	// write prescaler value
	if(mode == RTC_RANGE_MODE_HZ)
	{
		RTC->PRER = 0x002503E8; //37kHz/37(0X25)=1Khz, 1KHz/1000(0X3E8)=1Hz
		rtcRangeMode = RTC_RANGE_MODE_HZ;
	}
	else
	{
			RTC->PRER = 0x25; //37kHz/37(0X25)=1Khz
			rtcRangeMode = RTC_RANGE_MODE_KHZ;
	}
	// disable wakeup counter
	RTC->CR &= ~RTC_CR_WUTE;
	// allow access RTC
	rtc_disAllow();
}

void rtc_DeInit()
{
	if(!isRTC_Init) return;
	// allow access RTC
	rtc_allow();
	// disable rtc in RCC_CSR
	RCC->CSR &= ~RCC_CSR_RTCEN;
	// disallow RTC
	rtc_disAllow();
	// disable LSI
	//RCC->CSR &= ~RCC_CSR_LSION; 
	isRTC_Init = 0;
	

	if(!isRTC_Init) return;
	// 1. LSI setup and set as clock source
	// Enable LSI
//	printf("[INFO]DeInit_RTC ");
	RCC->CSR &= ~RCC_CSR_LSION; 
  // waiting LSI setup 
	isRTC_Init = 0;
}


void rtc_wakeup_interrupt_setting(float second)
{
	// Init RTC
	rtc_Init();
	uint16_t delayValue=0;
	if (second > 60)
	{
		//change setting to HZ
		rtc_chagePrescaler(RTC_RANGE_MODE_HZ);
		// change wakeup time to 1Hz
		delayValue = second;
	}
	else if (second >= 0.001)
	{
			//change setting to KHZ
		rtc_chagePrescaler(RTC_RANGE_MODE_KHZ);
		// change wakeup time to 1Hz
		delayValue = second*1000;
	}
	
	// allow access RTC register
	rtc_allow();
	// disable wakeup timer to modify it
	RTC->CR &=~ RTC_CR_WUTE; 
	// waiting until allow to modify reload value
	while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF);
	RTC->WUTR = delayValue;


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
	NVIC_SetPriority(RTC_IRQn, RTC_WAKEUP_INTERRUPT_PRIORITY);                     // set highest interrupt Priority for RTC EXTI (core)
	NVIC_EnableIRQ(RTC_IRQn);                          // enable  RTC EXTI (core)
}

void rtc_disable_wake_up_counter()
{
		// allow access RTC
	rtc_allow();
	// disable wakeup counter
	RTC->CR &= ~RTC_CR_WUTE;
	// disallow access RTC
	rtc_disAllow();
}

void rtc_clear_wakeup_flag()
{
		// clear wakeup flag
	rtc_allow();
	RTC->ISR &= ~RTC_ISR_WUTF;
	rtc_disAllow();
}