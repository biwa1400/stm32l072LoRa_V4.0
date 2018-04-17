#include "stm32l0xx.h"
#include "stdio.h"

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
	printf("[INFO]Init_RTC ");
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
	RCC->CSR &= ~RCC_CSR_RTCSEL;
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
	//RTC->PRER = 0x00250000; //37kHz/37(0X25)=1Khz
	// disable wakeup counter
	RTC->CR &= ~RTC_CR_WUTE;
	// change wakeup clock selection
	RTC->CR &= ~RTC_CR_WUCKSEL;
	while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF);
	RTC->CR |= (RTC_CR_WUCKSEL_2);
	// exit initalization mode
	RTC->ISR &= ~RTC_ISR_INIT;
	// disallow RTC
	rtc_disAllow();
	isRTC_Init = 1;
}

void rtc_DeInit()
{
	if(!isRTC_Init) return;
	// 1. LSI setup and set as clock source
	// Enable LSI
	printf("[INFO]DeInit_RTC ");
	RCC->CSR &= ~RCC_CSR_LSION; 
  // waiting LSI setup 
	isRTC_Init = 0;
}

void RTC_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PIF20;
}
