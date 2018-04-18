#include "regLib.h"

#define Bitfield_Modify(Result, Mask, Value) ((Result) = ((Result) & ~(Mask)) | ((Value) & (Mask)))

// GPIO CONFIG
void GPIO_Pin_Mode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Mode)
{
  Bitfield_Modify (GPIOx->MODER, GPIO_MODER_MODE0_Msk << (Pin * 2), Mode << (Pin * 2));
}


void GPIO_Pin_Pull (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Pull)
{
  Bitfield_Modify (GPIOx->PUPDR, GPIO_PUPDR_PUPD0_Msk << (Pin * 2), Pull << (Pin * 2));
}


void GPIO_Pin_Speed (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Speed)
{
  Bitfield_Modify (GPIOx->OSPEEDR, GPIO_OSPEEDER_OSPEED0_Msk << (Pin * 2), Speed << (Pin * 2));
}


void GPIO_Pin_Set (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->BSRR = 1 << Pin;
}


void GPIO_Pin_Clear (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->BRR = 1 << Pin;
}


void GPIO_Pin_Toggle (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->ODR ^= 1 << Pin;
}


void GPIO_Pin_Output (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Output)
{
  Bitfield_Modify (GPIOx->OTYPER, 0x1 << Pin, Output << Pin);
}


void GPIO_Alternate_Function (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t AF)
{
  Bitfield_Modify (GPIOA->AFR[Pin >> 3], 0xF << ((Pin & 0x7) * 4), AF << ((Pin & 0x7) * 4));
}

void GPIO_analogIn_lowPower()
{
		GPIOA->MODER = 0x00;
		GPIOB->MODER = 0x00;
		GPIOC->MODER = 0x00;
		GPIOD->MODER = 0x00;
		GPIOE->MODER = 0x00;
		GPIOH->MODER = 0x00;
}



