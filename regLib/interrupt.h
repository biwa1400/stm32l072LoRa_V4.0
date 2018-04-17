#ifndef __INTERRUPT_H
#define __INTERRUPT_H

typedef struct
{
	GPIO_TypeDef * GPIO_GROUP;
	uint32_t pin;
	uint32_t isRisingtTrig;
	uint32_t isFallingtTrig;
	uint32_t interruptPriority;
} IO_Interrupt_Def;

void IO_Interrupt_enable(IO_Interrupt_Def* io_interrupt_str);
void IO_Interrupt_enable_2();

#endif