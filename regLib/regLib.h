#ifndef __REGLIB_H
#define __REGLIB_H

#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
  
  #define __NOINLINE __attribute__ ( (noinline) ) 

#endif /* __GNUC__ */

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif


#include "stm32l0xx.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include "lowpower.h"
#include "rtc.h"
#include "gpio.h"
#include "spi.h"
#include "rng.h"
#include "interrupt.h"

#endif