#include "regLib.h"

uint32_t RNG_getRadonValue()
{
	// enable RNG clock
	RCC->AHBENR |= RCC_AHBENR_RNGEN;
	// enable RNG
	RNG->CR |= RNG_CR_RNGEN;
	// waiting value generate
	while(!(RNG->SR & RNG_SR_DRDY));
	// read Value
	uint32_t value =  RNG -> DR;
	// disable RNG
	RNG->CR &= ~RNG_CR_RNGEN;
	RCC->AHBENR &= ~RCC_AHBENR_RNGEN;  
	
	return value;
}