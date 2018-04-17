#include "regLib.h"
#include <string.h>

static inline uint8_t SPI_waitingBusy()
{
	while((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY);
	return 1;
}

static inline uint8_t SPI_waitingTxEmpty()
{
	while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE );
	return 1;
}

static inline uint8_t SPI_waitingRxReady()
{
	while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE );
	return 1;
}

void SPI_Init (SPI_TypeDef * SPI, SPI_INIT_DEF* spi_init_str)
{
	// 1.Enable PA,PB
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;  
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN; 
	RCC->IOPENR |= RCC_IOPENR_GPIODEN;  
	RCC->IOPENR |= RCC_IOPENR_GPIOEEN; 
	
  // 2.config GPIO mode to alternate 
	GPIO_Pin_Mode (spi_init_str->GPIO_GROUP_MISO, spi_init_str->GPIO_PIN_MISO, GPIO_MODE_AF_PP);
	GPIO_Pin_Mode (spi_init_str->GPIO_GROUP_MOSI, spi_init_str->GPIO_PIN_MOSI, GPIO_MODE_AF_PP);
	GPIO_Pin_Mode (spi_init_str->GPIO_GROUP_NSS, spi_init_str->GPIO_PIN_NSS, GPIO_MODE_AF_PP);
	GPIO_Pin_Mode (spi_init_str->GPIO_GROUP_SCLK, spi_init_str->GPIO_PIN_SCLK, GPIO_MODE_AF_PP);
		
	// 3.config GPIO speed	
	GPIO_Pin_Speed (spi_init_str->GPIO_GROUP_MISO, spi_init_str->GPIO_PIN_MISO, GPIO_SPEED_FREQ_VERY_HIGH);
	GPIO_Pin_Speed (spi_init_str->GPIO_GROUP_MOSI, spi_init_str->GPIO_PIN_MOSI, GPIO_SPEED_FREQ_VERY_HIGH);
	GPIO_Pin_Speed (spi_init_str->GPIO_GROUP_NSS, spi_init_str->GPIO_PIN_NSS, GPIO_SPEED_FREQ_VERY_HIGH);
	GPIO_Pin_Speed (spi_init_str->GPIO_GROUP_SCLK, spi_init_str->GPIO_PIN_SCLK, GPIO_SPEED_FREQ_VERY_HIGH);
	
  // 4.config alternate number -> 0
	GPIO_Alternate_Function (spi_init_str->GPIO_GROUP_MISO, spi_init_str->GPIO_PIN_MISO,spi_init_str->GPIO_AF_MISO );
	GPIO_Alternate_Function (spi_init_str->GPIO_GROUP_MOSI, spi_init_str->GPIO_PIN_MOSI, spi_init_str ->GPIO_AF_MOSI);
	GPIO_Alternate_Function (spi_init_str->GPIO_GROUP_NSS, spi_init_str->GPIO_PIN_NSS, spi_init_str ->GPIO_AF_NSS);
	GPIO_Alternate_Function (spi_init_str->GPIO_GROUP_SCLK, spi_init_str->GPIO_PIN_SCLK, spi_init_str ->GPIO_AF_SCLK);
	
	// 5. pull up PA15-> SPI1_NSS
	GPIO_Pin_Pull(spi_init_str->GPIO_GROUP_NSS, spi_init_str->GPIO_PIN_NSS,GPIO_PULLUP);
	
	// 5.Disable PA-PE model
	RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN;
	RCC->IOPENR &= ~RCC_IOPENR_GPIOCEN; 
	RCC->IOPENR &= ~RCC_IOPENR_GPIODEN; 
	RCC->IOPENR &= ~RCC_IOPENR_GPIOEEN; 	

	// spi setting
	//	1. reset SPI1 Model
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	// 1. Enable SPI1 Model
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// 3. NSS setting
	SPI -> CR1 &= ~(SPI_CR1_SSM);
	SPI -> CR2 |= SPI_CR2_SSOE;
	// 4. setting to master
	SPI->CR1 = SPI_CR1_MSTR;
	// 5. baudrate /2
	SPI->CR1 &= ~ SPI_CR1_BR;
	SPI->CR1 |= (0x00<<SPI_CR1_BR_Pos);
}

 
void SPI_LoRa_Init ()
{
	// setting GPIO 
	// PA6  -> SPI1_MISO
	// PA7  -> SPI1_MOSI
	// PA15 -> SPI1_NSS
	// PB3  -> SPI1_SCLK
	// 1.Enable PA,PB
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;  
  // 2.config GPIO mode to alternate 
	GPIOA -> MODER &= ~(GPIO_MODER_MODE6|GPIO_MODER_MODE7|GPIO_MODER_MODE15);  
	GPIOA -> MODER |= (GPIO_MODER_MODE6_1|GPIO_MODER_MODE7_1|GPIO_MODER_MODE15_1);
	
	GPIOB -> MODER &= ~GPIO_MODER_MODE3;  
	GPIOB -> MODER |= GPIO_MODER_MODE3_1;
	
	// 3.config GPIO speed	
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEED6|GPIO_OSPEEDER_OSPEED7|GPIO_OSPEEDER_OSPEED15);
	 		
	GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEED3;
	
  // 4.config alternate number -> 0
	GPIOA -> AFR[0] &= ~(uint32_t)(0xff<<4*6);
	GPIOA -> AFR[1] &= ~(uint32_t)(0x0f<<4*7);
	GPIOB -> AFR[0] &= ~(uint32_t)(0x0f<<4*3);
	
	// 5. pull up PA15-> SPI1_NSS
	GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPD15);
	GPIOA -> PUPDR |= (0X01<<GPIO_PUPDR_PUPD15_Pos);
	// 5.Disable PA model
	RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN; 

	// spi setting
	//	1. reset SPI1 Model
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	// 1. Enable SPI1 Model
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// 3. NSS setting
	SPI1 -> CR1 &= ~(SPI_CR1_SSM);
	SPI1 -> CR2 |= SPI_CR2_SSOE;
	// 4. setting to master
	SPI1->CR1 = SPI_CR1_MSTR;
	// 5. baudrate /2
	SPI1->CR1 &= ~ SPI_CR1_BR;
	SPI1->CR1 |= (0x00<<SPI_CR1_BR_Pos);
}

uint8_t SPI_RegisterRead (SPI_TypeDef * SPI, uint8_t address)
{
	
	// send read address
	if(SPI_waitingBusy()) 
	{
		// change to  16 bits
		SPI->CR1|=SPI_CR1_DFF;
		//enable transmit
		SPI -> CR1 |= SPI_CR1_SPE;
	   // Will inititiate 8-bit transmission if TXE */
		// clear first byte to 0 for read
		address &= 0x7F;
		SPI->DR = address<<8;
	}
	
	//read
	// 1. waiting read
	SPI_waitingRxReady();
	uint8_t readData = SPI1->DR;
	//disable transmit
	SPI -> CR1 &= ~SPI_CR1_SPE;
	return readData;
}





void SPI_RegisterWrite (SPI_TypeDef * SPI,uint8_t address,uint8_t value)
{
	// send
	if(SPI_waitingBusy())
	{
		// change to  16 bits
		SPI->CR1|=SPI_CR1_DFF;
		//enable transmit
		SPI -> CR1 |= SPI_CR1_SPE;
	   // Will inititiate 8-bit transmission if TXE */
		uint16_t sendData = ((uint16_t)address|0x80)<<8|value;
		//printf("send: %.4x",sendData);
		SPI->DR = sendData;
	}

	// 1. waiting read
	SPI_waitingRxReady();
	//disable transmit
	SPI -> CR1 &= ~SPI_CR1_SPE;
	// clear DR register
	SPI->DR;
}


void SPI_BlockWrite (SPI_TypeDef * SPI,uint8_t address, uint8_t* value, uint8_t size)
{
	// send
	if(SPI_waitingBusy())
	{
		// change to  8 bits
		SPI->CR1&= ~SPI_CR1_DFF;
		//enable transmit
		SPI -> CR1 |= SPI_CR1_SPE;
		uint8_t sendData = address|0x80;
		SPI->DR = sendData;
	}

	for (uint8_t i=0;i<size;i++)
	{
		if(SPI_waitingTxEmpty())
		{
				SPI->DR = value[i];
		}
	}
	
	SPI_waitingRxReady();
	//read
	// 1. waiting read
	SPI_waitingRxReady();
	//disable transmit
	SPI -> CR1 &= ~SPI_CR1_SPE;
	// clear receive flag
	SPI->DR;
}

void SPI_BlockRead (SPI_TypeDef * SPI,uint8_t address, uint8_t* value, uint8_t size)
{
	// send
	if(SPI_waitingBusy())
	{
		// change to  8 bits
		SPI->CR1&= ~SPI_CR1_DFF;
		//enable transmit
		SPI -> CR1 |= SPI_CR1_SPE;
	   // Will inititiate 8-bit transmission if TXE */
		uint8_t sendData = address& ~0x80;
		SPI->DR = sendData;
		// clear receive flag
		SPI_waitingRxReady();
		SPI->DR;
	}

	for (uint8_t i=0;i<size;i++)
	{
		if(SPI_waitingTxEmpty())
		{
			SPI->DR = 0x00;
		}
		SPI_waitingRxReady();
		value[i] = SPI->DR;
	}
	//disable transmit
	SPI -> CR1 &= ~SPI_CR1_SPE;
}




