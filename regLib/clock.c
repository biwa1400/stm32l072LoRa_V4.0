#include "regLib.h"

volatile uint32_t speed_APB1=MSI_FREQUENCY;
volatile uint32_t speed_HCLK=MSI_FREQUENCY;

// weak for calling by special modle
__weak void update_clock_DBPORT(void)
{
}

void update_All_Clock()
{
	update_clock_DBPORT();
}

// clock speed
static void changeALLfrequency(uint32_t frqeuency)
{
	speed_APB1=frqeuency;
  speed_HCLK=frqeuency;
	update_All_Clock();
}

inline uint32_t getSpeed_APB1()
{
	return speed_APB1;
}

inline uint32_t getSpeed_HCLK()
{
	return speed_HCLK;
}

// switch
void switch_32MHz()
{
	// judge it it current setting
	if((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)
	{
		return;
	}
	
	// judge HSIRDY is ON
	if((RCC->CR & RCC_CR_HSIRDY) == 0)
	{
		//enable HSI
		RCC->CR |= RCC_CR_HSION;
		// waiting HSI16 ready
		while((RCC->CR & RCC_CR_HSIRDY) == 0);
	}
	
	//setup PLL
	//1. disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	//2. wait PLL stop
	while((RCC->CR & RCC_CR_PLLRDY) != 0);
	//3.select PLL source to HSI16
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI;
	//4. mul
	RCC->CFGR &= ~RCC_CFGR_PLLMUL;
	RCC->CFGR |= RCC_CFGR_PLLMUL8;
	//5. div
	RCC->CFGR &= ~RCC_CFGR_PLLDIV;
	RCC->CFGR |= (RCC_CFGR_PLLDIV_0|RCC_CFGR_PLLDIV_1);
	// 6. set up PLL
	RCC->CR |= RCC_CR_PLLON;
	// 7. wait PLL setup finish
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	// set Flash latency to 1 wait state
	FLASH->ACR |= FLASH_ACR_LATENCY;
	
	// switch system clock to PLL
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// wait switch stable
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
	// disable MSI
	RCC->CR &= ~RCC_CR_MSION;
	
	// change wake up clock source
	RCC->CFGR |= RCC_CFGR_STOPWUCK;
	
	// change var frequency
	changeALLfrequency(PLL32_FREQUENCY);
}

void switch_16MHz()
{
	if((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI)
	{
		return;
	}

	//enable HSI
	RCC->CR |= RCC_CR_HSION;
	// waiting HSI16 ready
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// switch system clock to HSI16
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	
	// wait switch stable
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
	
	// set Flash latency to 0 wait state
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	
	//disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	
	// disable MSI
	RCC->CR &= ~RCC_CR_MSION;
	
	// change wake up clock source
	RCC->CFGR |= RCC_CFGR_STOPWUCK;
	
	// change var frequency
	changeALLfrequency(HSI_FREQUENCY);
	
}

void switch_2MHz()
{
	// judge it it current setting
	if((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_MSI)
	{
		return;
	}
	
	//enable MSI
	RCC->CR |= RCC_CR_MSION;
	// waiting MSI ready
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	// switch system clock to MSI
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	// wait switch stable
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SW_MSI);
	
	// set Flash latency to 0 wait state
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	
	//disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	
	//disable HSI16
	RCC->CR &= ~RCC_CR_HSION;
	
	// change wake up clock source
	RCC->CFGR &= ~RCC_CFGR_STOPWUCK;
	
	//change var frequency
	changeALLfrequency(MSI_FREQUENCY);
}



static void setClock2HSI_16MHz()
{
	// HSI
	//enable HSI init interrupt in NVIC
	NVIC_EnableIRQ(RCC_CRS_IRQn);
	NVIC_SetPriority(RCC_CRS_IRQn, 0);
	//enable HSI init interrupt in RCC
	RCC->CIER |= RCC_CIER_HSIRDYIE; 
	//enable HSI
	RCC->CR |= RCC_CR_HSION;
}

static void setPLL_HSI_32MHz()
{
	//1. disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	//2. wait PLL stop
	while((RCC->CR & RCC_CR_PLLRDY) != 0);
	// select PLL source
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	//3. set Flash latency to 1 wait state
	FLASH->ACR |= FLASH_ACR_LATENCY;
	//4. mul
	RCC->CFGR &= ~RCC_CFGR_PLLMUL;
	RCC->CFGR |= RCC_CFGR_PLLMUL8;
	//5. div
	RCC->CFGR &= ~RCC_CFGR_PLLDIV;
	RCC->CFGR |= (RCC_CFGR_PLLDIV_0|RCC_CFGR_PLLDIV_1);
	// 6. set up PLL
	RCC->CR |= RCC_CR_PLLON;
	// 7. wait PLL setup finish
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);
}

void RCC_CRS_IRQHandler(void)
{
 // Check the flag HSI ready 
 if ((RCC->CIFR & RCC_CIFR_HSIRDYF) != 0)
 {
	 // clear Interrupt Flag
	 RCC->CICR |= RCC_CICR_HSIRDYC; 
	 // switch clock to HSI
	 RCC->CFGR = ((RCC->CFGR & (~RCC_CFGR_SW)) |  RCC_CFGR_SW_HSI);
	 // wait switch stable
	 while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
	 // disable interrupt
	 NVIC_DisableIRQ(RCC_CRS_IRQn);
 }
 else
 {
 /* Manage error */
 }
}

void clock_disable_LSI()
{
	RCC->CSR &= ~RCC_CSR_LSION; 
}


void PA_8_clock_out_enable(uint32_t clock_source, uint32_t div)
{
	// configGPIO
	// enable GPIOA clock
	RCC->IOPENR |= RCC_IOPENR_IOPAEN;
	// set PA_8 to alternate mode
	GPIOA->MODER &= ~GPIO_MODER_MODE8_Msk; 
	GPIOA->MODER |=  GPIO_MODER_MODE8_1; 
	// disable GPIOA clock
	RCC->IOPENR &= ~RCC_IOPENR_IOPAEN;
	
	// config RCC MCO
	// choose MCO source to SYSCLK
	RCC->CFGR &= ~RCC_CFGR_MCOSEL_Msk;
	RCC->CFGR |= clock_source;
	// choose MCO prescaler to div
	RCC->CFGR &= ~RCC_CFGR_MCOPRE_Msk;
	RCC->CFGR |=  div;
}

