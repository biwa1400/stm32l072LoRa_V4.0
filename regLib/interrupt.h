#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#define I2C1_INTERRUPT_PRIORITY 2U
#define DELAY_INTERRUPT_PRIORITY 0U
#define RTC_WAKEUP_INTERRUPT_PRIORITY 1U
#define LPTIM1_INTERRUPT_PRIORITY 1U
#define STATE_MACHINE_PRIORITY 9U

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
void Interrupt_soft_EXTI_init(uint8_t trigger_Num, uint8_t priority);
void Interrupt_soft_EXTI_trigger(uint8_t trigger_Num);

#endif