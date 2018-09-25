#ifndef __CLOCK_H
#define __CLOCK_H
#include "stm32l0xx.h"


#define MSI_FREQUENCY 2097000UL
#define HSI_FREQUENCY 16000000UL
#define PLL32_FREQUENCY 32000000UL

// get Frequency
uint32_t getSpeed_APB1();
uint32_t getSpeed_HCLK();

// speed change
void switch_32MHz();
void switch_16MHz();
void switch_2MHz();

void clock_disable_LSI();

//PA8_CLOCK_OUT
// source
#define PA8_CLOCK_OUT_SOURCE_SYSCLK           (0x01U<<RCC_CFGR_MCOSEL_Pos)
#define PA8_CLOCK_OUT_SOURCE_HSI16            (0x02U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_MSI              (0x03U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_HSE              (0x04U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_PLL              (0x05U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_LSI              (0x06U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_LSE              (0x07U<<RCC_CFGR_MCOSEL_Pos)  
#define PA8_CLOCK_OUT_SOURCE_HSI48            (0x08U<<RCC_CFGR_MCOSEL_Pos)  

// div
#define PA8_CLOCK_OUT_DIV1                     (0x00U<<RCC_CFGR_MCOPRE_Pos)  
#define PA8_CLOCK_OUT_DIV2                     (0x01U<<RCC_CFGR_MCOPRE_Pos)                 
#define PA8_CLOCK_OUT_DIV3                     (0x02U<<RCC_CFGR_MCOPRE_Pos)                  
#define PA8_CLOCK_OUT_DIV8                     (0x03U<<RCC_CFGR_MCOPRE_Pos)                 
#define PA8_CLOCK_OUT_DIV16                    (0x04U<<RCC_CFGR_MCOPRE_Pos)

void PA_8_clock_out_enable(uint32_t clock_source, uint32_t div);



#endif
