#include "regLib.h"
#include "stdio.h"

void i2c1_enable_NVIC_interrupt()
{
	NVIC_EnableIRQ(I2C1_IRQn); 
	NVIC_SetPriority(I2C1_IRQn,I2C1_INTERRUPT_PRIORITY); 
}

void i2c1_init()
{
	// config I/O
	//Enable PB_8->SCL PB_9->SDA
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;  
	
 // config GPIO mode to alternate  
	GPIOB -> MODER &= ~GPIO_MODER_MODE8;  
	GPIOB -> MODER &= ~GPIO_MODER_MODE9;
	
	GPIOB -> MODER |= (GPIO_MODER_MODE8_1);
	GPIOB -> MODER |= (GPIO_MODER_MODE9_1);
	
	// config GPIO speed
	GPIOB -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEED8;
	GPIOB -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEED9;
	
	GPIOB -> OSPEEDR |= (GPIO_OSPEEDER_OSPEED8_0|GPIO_OSPEEDER_OSPEED8_1);
	GPIOB -> OSPEEDR |= (GPIO_OSPEEDER_OSPEED9_0|GPIO_OSPEEDER_OSPEED9_1);
	
 // config alternate number -> 4
	GPIOB -> AFR[1] &= ~(uint32_t)(0x0f<<(0*4));
	GPIOB -> AFR[1] &= ~(uint32_t)(0x0f<<(1*4));
	
	GPIOB -> AFR[1] |=  (uint32_t)(0x04<<(0*4));
	GPIOB -> AFR[1] |=  (uint32_t)(0x04<<(1*4));
	
	// open drain
	GPIOB ->OTYPER |= GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9;
	
	//Disable PB_8 PB_9
	RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN; 	
	
	
	
	// I2C1 400Khz
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	//I2C1->TIMINGR = (uint32_t) 0x00300619;
	I2C1->TIMINGR = (uint32_t) 0x0030baba;
	I2C1->CR2 |= I2C_CR2_AUTOEND;
	
	
	//RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
	
	// enable interrupt
	i2c1_enable_NVIC_interrupt();
}

void i2c1_transmit_single_byte (uint8_t addr, uint8_t byte)
{
	//RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 |= I2C_CR1_PE;
	// send bytes
	I2C1->CR2 = (1<<16) | (addr<<1);

		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
			I2C1->TXDR = byte; /* Byte to send */
			I2C1->CR2 |= I2C_CR2_START; /* Go */
		}

  	//RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
}

static uint8_t*  recvBuf =0;
static uint8_t  recvSize  = 0;
static uint8_t  recvBuf_index = 0;
static void (*receive_finishCallback)(uint8_t* recvBuf);

void i2c1_receive(uint8_t addr, uint8_t bytes,uint8_t* buf, void (*receiveCallback)(uint8_t* recvBuf))
{
	recvBuf = buf;
	recvSize = bytes;
	recvBuf_index = 0;
	receive_finishCallback = receiveCallback;
	
	I2C1->CR1 |= I2C_CR1_PE;
	//RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	//while((RCC->APB1ENR & RCC_APB1ENR_I2C1EN) != RCC_APB1ENR_I2C1EN);
	//delay(0.001);

	// enable RXNE interrupt
	I2C1->CR1 |= I2C_CR1_RXIE;
	
	I2C1->CR2 = I2C_CR2_AUTOEND | (bytes<<16) | I2C_CR2_RD_WRN |(addr<<1); /* (3) */
	I2C1->CR2 |= I2C_CR2_START; /* Go */

	//I2C2->RXDR;
	
  //RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
}

uint8_t getRecvBuf_index(){
	return recvBuf_index;
}

void I2C1_IRQHandler()
{
	printf("interrupt!\n");
	if((I2C1->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE)
	{
		recvBuf [recvBuf_index++]=I2C1->RXDR;
	}
	// receive finish
	if((I2C1->ISR & I2C_ISR_STOPF) == I2C_ISR_STOPF && recvBuf_index==recvSize){
		I2C1->ICR |= I2C_ICR_STOPCF;
		recvSize = 0;
		recvBuf_index = 0;
		receive_finishCallback(recvBuf);
	}
}
