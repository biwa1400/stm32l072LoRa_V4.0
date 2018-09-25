#include "regLib.h"
#include "sx1276.h"
#include "loraChipPinDefine.h"
#include "loraChip.h"
#include <stdio.h>


static void LoRa_Init_GPIO()
{
		// 1.Enable PA,PB,PC
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
	
	// RADIO_TCXO_VCC_PORT
	GPIO_Pin_Mode (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, RADIO_TCXO_VCC_MODE);
  GPIO_Pin_Pull (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, RADIO_TCXO_VCC_PULL);
  GPIO_Pin_Speed (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, RADIO_TCXO_VCC_SPEED);
  //GPIO_Pin_Clear (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN);
	
	// Antena switch RX
	GPIO_Pin_Mode (RADIO_ANT_SWITCH_RX_PORT, RADIO_ANT_SWITCH_RX_PIN, RADIO_ANT_SWITCH_RX_MODE);
  GPIO_Pin_Pull (RADIO_ANT_SWITCH_RX_PORT, RADIO_ANT_SWITCH_RX_PIN, RADIO_ANT_SWITCH_RX_PULL);
  GPIO_Pin_Speed (RADIO_ANT_SWITCH_RX_PORT, RADIO_ANT_SWITCH_RX_PIN, RADIO_ANT_SWITCH_RX_SPEED);
  GPIO_Pin_Clear (RADIO_ANT_SWITCH_RX_PORT, RADIO_ANT_SWITCH_RX_PIN);

	// Antena switch TX_BOOST
  GPIO_Pin_Mode (RADIO_ANT_SWITCH_TX_BOOST_PORT, RADIO_ANT_SWITCH_TX_BOOST_PIN, RADIO_ANT_SWITCH_TX_BOOST_MODE);
  GPIO_Pin_Pull (RADIO_ANT_SWITCH_TX_BOOST_PORT, RADIO_ANT_SWITCH_TX_BOOST_PIN, RADIO_ANT_SWITCH_TX_BOOST_PULL);
  GPIO_Pin_Speed (RADIO_ANT_SWITCH_TX_BOOST_PORT, RADIO_ANT_SWITCH_TX_BOOST_PIN, RADIO_ANT_SWITCH_TX_BOOST_SPEED);
  GPIO_Pin_Clear (RADIO_ANT_SWITCH_TX_BOOST_PORT, RADIO_ANT_SWITCH_TX_BOOST_PIN);

	// Antena switch TX_RFO
  GPIO_Pin_Mode (RADIO_ANT_SWITCH_TX_RFO_PORT, RADIO_ANT_SWITCH_TX_RFO_PIN, RADIO_ANT_SWITCH_TX_RFO_MODE);
  GPIO_Pin_Pull (RADIO_ANT_SWITCH_TX_RFO_PORT, RADIO_ANT_SWITCH_TX_RFO_PIN, RADIO_ANT_SWITCH_TX_RFO_PULL);
  GPIO_Pin_Speed (RADIO_ANT_SWITCH_TX_RFO_PORT, RADIO_ANT_SWITCH_TX_RFO_PIN, RADIO_ANT_SWITCH_TX_RFO_SPEED);
  GPIO_Pin_Clear (RADIO_ANT_SWITCH_TX_RFO_PORT, RADIO_ANT_SWITCH_TX_RFO_PIN);
	
	
	// reset pin
	GPIO_Pin_Mode (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_MODE);
  GPIO_Pin_Pull (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_PULL);
  GPIO_Pin_Speed (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_SPEED);
  GPIO_Pin_Set (RADIO_RESET_PORT, RADIO_RESET_PIN);
	
	// Disable PA,PB,PC
	RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN; 
	RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN;
	RCC->IOPENR &= ~RCC_IOPENR_GPIOCEN;
	
	
	// Config IO interrupt
	// DIO 0
	IO_Interrupt_Def io_int_str;
	io_int_str.GPIO_GROUP=RADIO_DIO0_PORT;
	io_int_str.pin = RADIO_DIO0_PIN;
	io_int_str.isRisingtTrig = TRUE;
	io_int_str.interruptPriority = RADIO_DIO_0_INTERRUPT_PRIORITY;
	IO_Interrupt_enable(&io_int_str);
	
	// DIO 1
	io_int_str.GPIO_GROUP=RADIO_DIO1_PORT;
	io_int_str.pin = RADIO_DIO1_PIN;
	io_int_str.isRisingtTrig = TRUE;
	io_int_str.interruptPriority = RADIO_DIO_1_INTERRUPT_PRIORITY;
	IO_Interrupt_enable(&io_int_str);
	
}

static void LoRa_Init_SPI()
{
	SPI_INIT_DEF spi_init_str;
	// MOSI PA7
	spi_init_str.GPIO_GROUP_MOSI = RADIO_MOSI_PORT;
	spi_init_str.GPIO_PIN_MOSI = RADIO_MOSI_PIN;
	spi_init_str.GPIO_AF_MOSI = RADIO_MOSI_AF;
	// MISO PA6
	spi_init_str.GPIO_GROUP_MISO = RADIO_MISO_PORT;
	spi_init_str.GPIO_PIN_MISO = RADIO_MISO_PIN;
	spi_init_str.GPIO_AF_MISO = RADIO_MISO_AF;
	// NSS PA15
	spi_init_str.GPIO_GROUP_NSS = RADIO_NSS_PORT;
	spi_init_str.GPIO_PIN_NSS = RADIO_NSS_PIN;
	spi_init_str.GPIO_AF_NSS = RADIO_NSS_AF;
	// SCLK PB3
	spi_init_str.GPIO_GROUP_SCLK = RADIO_SCLK_PORT;
	spi_init_str.GPIO_PIN_SCLK = RADIO_SCLK_PIN;
	spi_init_str.GPIO_AF_SCLK = RADIO_SCLK_AF;
	
	SPI_Init (RADIO_SPI, &spi_init_str);
}



static inline void RegisterWrite(uint8_t address,uint8_t value)
{
	SPI_RegisterWrite (RADIO_SPI,address,value);
}

static inline uint8_t RegisterRead(uint8_t address)
{
	return SPI_RegisterRead (RADIO_SPI, address);
}

static inline void BlockWrite(uint8_t address, uint8_t* value, uint8_t size)
{
	SPI_BlockWrite (RADIO_SPI, address, value, size);
}

static inline void BlockRead(uint8_t address, uint8_t* value, uint8_t size)
{
	SPI_BlockRead (RADIO_SPI,address,value,size);
}

void RegisterModify(uint8_t address, uint32_t mask, uint32_t changeValue)
{
	uint8_t value = RegisterRead(address);
	value &= mask;
	value |= changeValue;
	RegisterWrite(address,value);
}

static void LoRa_Init_RegParameters()
{
	// waiting chip turn on 
	while((RegisterRead(SX1276_RegOPMODE)&RFLR_OPMODE_STANDBY)!=RFLR_OPMODE_STANDBY);
	//3. enter sleep mode to change LoRa Mode
	RegisterWrite(SX1276_RegOPMODE,RFLR_OPMODE_SLEEP);
	// setting it to LoRa mode
	RegisterWrite(0x01,0x80);
	
	// invert IQ
	RegisterWrite (SX1276_RegINVERTIQ, 0x67);
	// synchronous code
	RegisterWrite(SX1276_RegSYNCWORD,0x34);
	// enable CRC
	RegisterWrite(SX1276_RegMODEMCONFIG2,0x74);
	// symbol length exceeds 16ms
	RegisterWrite(SX1276_RegMODEMCONFIG3,0x08);
}

void LoRa_HW_init()
{	

	//1. init lora_gpio
	LoRa_Init_GPIO();
	//2. init lora_spi
	LoRa_Init_SPI();

	
	//1. set TCXO_VCC
	turnOn_TCXO();
	
	// waiting chip turn on 
	while((RegisterRead(SX1276_RegOPMODE)&RFLR_OPMODE_STANDBY)!=RFLR_OPMODE_STANDBY);
	//3. enter sleep mode to change LoRa Mode
	RegisterWrite(SX1276_RegOPMODE,RFLR_OPMODE_SLEEP);
	
	// setting it to LoRa mode
	RegisterWrite(0x01,0x80);
	
	// invert IQ
	RegisterWrite (SX1276_RegINVERTIQ, 0x67);
	// synchronous code
	RegisterWrite(SX1276_RegSYNCWORD,0x34);
	// enable CRC
	RegisterWrite(SX1276_RegMODEMCONFIG2,0x74);
	// symbol length exceeds 16ms
	RegisterWrite(SX1276_RegMODEMCONFIG3,0x08);
	
	
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,0x06);
	delay(0.001);
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,RFLR_OPMODE_SLEEP);
	shutDown_TCXO();
}

void LoRa_wakeUp()
{
	turnOn_TCXO();
	
}

void LoRa_sleep()
{
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,RFLR_OPMODE_SLEEP);
	shutDown_TCXO();
}

void LoRa_standby()
{
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,RFLR_OPMODE_STANDBY);
	shutDown_TCXO();
}

void LoRa_resetChip()
{
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN; 
  GPIO_Pin_Clear (RADIO_RESET_PORT, RADIO_RESET_PIN);
  delay (5);
  GPIO_Pin_Set (RADIO_RESET_PORT, RADIO_RESET_PIN);
  //Save power. Reset pin is pulled up in SX1276.
  GPIO_Pin_Mode (RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_MODE_INPUT);
  delay(5);
	RCC->IOPENR &= ~RCC_IOPENR_GPIOCEN;
}


void LoRa_Set_TxPower (uint8_t value)
{
	RegisterWrite(SX1276_RegPACONFIG,value|0x80);
}

void LoRa_Set_SF (uint8_t value)
{
	uint8_t oldValue = RegisterRead(SX1276_RegMODEMCONFIG2);
	oldValue &= ~(0xf0);
	oldValue |= value<<4;
	RegisterWrite(SX1276_RegMODEMCONFIG2,oldValue);
//	printf("set value to %d %.2x\n",value,oldValue);
	
	//change low datarate optimize
	uint8_t registerValue = RegisterRead(SX1276_RegMODEMCONFIG3);
	if(value <= 10 && (registerValue&0x08)!=0)
	{
		RegisterWrite(SX1276_RegMODEMCONFIG3,registerValue&(~0x08));
	}
	else if(value > 10 && (registerValue&0x08)==0)
	{
		RegisterWrite(SX1276_RegMODEMCONFIG3,registerValue|0x08);
	}
}

void LoRa_Set_Frequency (uint32_t Frequency)
{	
  uint64_t frf = ((uint64_t)Frequency << 19) / 32000000;
  RegisterWrite (SX1276_RegFRFMSB, (uint8_t)(frf >> 16));
  RegisterWrite (SX1276_RegFRFMID, (uint8_t)(frf >> 8));
  RegisterWrite (SX1276_RegFRFLSB, (uint8_t)(frf >> 0));
}

void RF_Send(uint8_t* data, uint8_t size)
{	
	// chage state to standBy
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,RFLR_OPMODE_STANDBY);

	// write value to register
  RegisterWrite (SX1276_RegFIFOADDRPTR, 0x80);
	BlockWrite(SX1276_RegFIFO,data,size);
  RegisterWrite (SX1276_RegPAYLOADLENGTH, size);

	// change IO Interrupt status
	RegisterModify(SX1276_RegDIOMAPPING1,RFLR_DIOMAPPING1_DIO0_MASK,RFLR_DIOMAPPING1_DIO0_01);
	// change mode to transmit
	RegisterModify(SX1276_RegOPMODE,RFLR_OPMODE_MASK,RFLR_OPMODE_TRANSMITTER);
}


void RF_Receive()
{
	// change chip state to receive Continuous
		// change IO Interrupt status
	RegisterModify(SX1276_RegDIOMAPPING1,RFLR_DIOMAPPING1_DIO0_MASK,RFLR_DIOMAPPING1_DIO0_00);
		// change mode to receive
	RegisterModify (SX1276_RegOPMODE, RFLR_OPMODE_MASK,RFLR_OPMODE_RECEIVER_SINGLE);
}

uint8_t LoRaChip_readReceiveBuf(uint8_t* bufPtr)
{
		uint8_t FIFO_addr = RegisterRead(SX1276_RegFIFORXCURRENTADDR);
		uint8_t rxLength = RegisterRead(SX1276_RegRXNBBYTES);
		RegisterWrite(SX1276_RegFIFOADDRPTR, FIFO_addr);
		BlockRead(SX1276_RegFIFO, bufPtr,rxLength);
	  return rxLength;
}


void LoRa_PrintSetting()
{
		for (uint8_t i=0;i<0x70;i++)
	{
		uint8_t value = RegisterRead (i);
		printf("%.2x : %.2x\n",i,value);
		delay(0.02);
	}
}

uint8_t getInterruptFlag()
{
	return RegisterRead(SX1276_RegIRQFLAGS);
}

int getRssi()
{
	return -157 + RegisterRead(SX1276_RegPKTRSSIVALUE);
}

void clearInterruptFlag(uint8_t interruptBit)
{
	RegisterWrite(SX1276_RegIRQFLAGS,interruptBit);
}

void turnOn_TCXO()
{
		RCC->IOPENR |= RCC_IOPENR_GPIOBEN; 
		GPIO_Pin_Mode (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, RADIO_TCXO_VCC_MODE);
		GPIO_Pin_Set (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN);
		RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN; 
}

void shutDown_TCXO()
{
		RCC->IOPENR |= RCC_IOPENR_GPIOBEN; 
		GPIO_Pin_Clear (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN);
		GPIO_Pin_Mode (RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, GPIO_MODE_ANALOG);
		RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN; 
}


