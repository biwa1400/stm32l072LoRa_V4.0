#include "stm32l0xx.h"
#include "stdio.h"
#include "config.h"

uint8_t dbPortInit = 0;

static void dbPort_enable()
{  	 
  // calculate boundRate
	float usartDiv=(float)(getSpeed_APB1())/(DEBUG_PORT_BAUDRATE*16);      // get USARTDIV
	uint16_t usartDiv_mantissa=usartDiv;				                //get USARTDIV mantissa
	uint16_t usartDiv_fraction=(usartDiv-usartDiv_mantissa)*16; //get USARTDIV fraction
  usartDiv_mantissa<<=4;                                      // matching USART_BRR
	uint16_t divValue = usartDiv_mantissa+usartDiv_fraction; 
	
	// config I/O
	//Enable PA_2->Tx
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;  
 // config GPIO mode to alternate  
	GPIOA -> MODER &= ~GPIO_MODER_MODE2;  
	GPIOA -> MODER |= (GPIO_MODER_MODE2_1);
	// config GPIO speed
	GPIOA -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEED2;  		
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEED2_0|GPIO_OSPEEDER_OSPEED2_1);
 // config alternate number -> 4
	GPIOA -> AFR[0] &= ~(uint32_t)(0x0f<<2*4);
	GPIOA -> AFR[0] |=  (uint32_t)(0x04<<2*4);
	//Disable PA_2->Tx
	RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN; 	
	
	
	// config USART2
	// enable USART2
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;  
	// reset USART2
	RCC->APB1RSTR|=RCC_APB1RSTR_USART2RST;   
	RCC->APB1RSTR &=~RCC_APB1RSTR_USART2RST;    	   
	
	
	//Config boundRate
 	USART2->BRR=divValue;
	//enable USART
	USART2->CR1 |=USART_CR1_UE;
	//enable Tx
	USART2->CR1 |=USART_CR1_TE;
	
	dbPortInit = 1;
}

void dbPort_disable()
{  	 	
	// config I/O to Analog In 
	//Enable PA_2->Tx
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;  
 // config GPIO mode to alternate  
	GPIOA -> MODER &= ~GPIO_MODER_MODE2;  
	GPIOA -> MODER |= (GPIO_MODER_MODE2_0|GPIO_MODER_MODE2_1);
	// convert GPIO speed -> low
	GPIOA -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEED2;  		
 // config alternate number -> 4
	
	// config USART2
	// enable USART2
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;  
	// reset USART2
	RCC->APB1RSTR|=RCC_APB1RSTR_USART2RST;   
	RCC->APB1RSTR &=~RCC_APB1RSTR_USART2RST;    
  // disable USART2
	RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; 
	dbPortInit = 0; 	
}




void uart_send(uint8_t data)
{
	if (dbPortInit == 1)
	{
		while(READ_BIT(USART2->ISR,USART_ISR_TXE) == 0);
		USART2->TDR = data;      	
	}
	else
	{
		dbPort_enable();
		uart_send(data);
	}
}

int fputc(int ch, FILE *f)
{      
	uart_send((uint8_t) ch);
	return ch;
}



void update_clock_DBPORT()
{
	dbPort_enable();
}