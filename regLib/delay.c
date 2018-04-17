#include "regLib.h"
uint32_t SysTick_delay_count = 0;
void SysTick_Handler(void)
{
	SysTick_delay_count--;
}

__STATIC_INLINE uint32_t Config_sysTick(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) { return (1UL); }    /* Reload value impossible */

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, DELAY_INTERRUPT_PRIORITY);                               /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

void delay(float s)
{
	SysTick_delay_count = s*1000;
	Config_sysTick(getSpeed_HCLK()/1000-1);
	// set Priority for Systick Interrupt

	while(SysTick_delay_count>0);
	// stop system tick
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;
}
