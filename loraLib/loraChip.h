#ifndef __LORA_CHIP_H
#define __LORA_CHIP_H

void RegisterModify(uint8_t address, uint32_t mask, uint32_t changeValue);

void LoRa_HW_init();

void LoRa_sleep();

void LoRa_standby();

void LoRa_resetChip();

void LoRa_Set_TxPower (uint8_t value);

void LoRa_Set_SF (uint8_t value);

void LoRa_Set_Frequency (uint32_t Frequency);

void RF_Send(uint8_t* data, uint8_t size);

void RF_Receive();

void LoRa_PrintSetting();

uint8_t getInterruptFlag();

void    clearInterruptFlag(uint8_t interruptBit);

uint8_t LoRaChip_readReceiveBuf(uint8_t* bufPtr);

void turnOn_TCXO();

void shutDown_TCXO();

void LoRa_wakeUp();

int getRssi();

#endif