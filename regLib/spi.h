#ifndef __SPI_H
#define __SPI_H

typedef struct
{
	GPIO_TypeDef * GPIO_GROUP_MISO; 
	GPIO_TypeDef * GPIO_GROUP_MOSI;
	GPIO_TypeDef * GPIO_GROUP_NSS;
	GPIO_TypeDef * GPIO_GROUP_SCLK; 
	uint32_t       GPIO_PIN_MISO;
	uint32_t       GPIO_PIN_MOSI;
	uint32_t       GPIO_PIN_NSS;
	uint32_t       GPIO_PIN_SCLK;
	uint32_t       GPIO_AF_MISO;
	uint32_t       GPIO_AF_MOSI;
	uint32_t       GPIO_AF_NSS;
	uint32_t       GPIO_AF_SCLK;
} SPI_INIT_DEF;


void SPI_Init (SPI_TypeDef * SPI, SPI_INIT_DEF* spi_init_str);

void SPI_LoRa_Init ();

uint8_t SPI_RegisterRead (SPI_TypeDef * SPI, uint8_t address);

void SPI_RegisterWrite (SPI_TypeDef * SPI,uint8_t address,uint8_t value);

void SPI_BlockWrite (SPI_TypeDef * SPI,uint8_t address, uint8_t* value, uint8_t size);

void SPI_BlockRead (SPI_TypeDef * SPI,uint8_t address, uint8_t* value, uint8_t size);


#endif