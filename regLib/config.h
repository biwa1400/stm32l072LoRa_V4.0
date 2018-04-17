#ifndef __CONFIG__H_
#define __CONFIG__H_

#include "regLib.h"
#include <stdbool.h>






//update
void updateSetting_dbPort();
void dbPort_disable();

// speed


// delay
void delay(float s);



// low Power
void stop(uint32_t second);

// clock
void clockOut();

//timer
inline void rtc_allow();
inline void rtc_disAllow();
inline void rtc_Init();

//gpio
// Pin Mode
#define  GPIO_MODE_INPUT                        0x00U
#define  GPIO_MODE_OUTPUT_PP                    0x01U
#define  GPIO_MODE_AF_PP                        0x02U
#define  GPIO_MODE_ANALOG                       0x03U

// Pin Speed
#define  GPIO_SPEED_FREQ_LOW                    0x00U // 0.4MHz
#define  GPIO_SPEED_FREQ_MEDIUM                 0x01U //0.4-2MHz
#define  GPIO_SPEED_FREQ_HIGH                   0x02U //2-10MHz
#define  GPIO_SPEED_FREQ_VERY_HIGH              0x03U //10-35MHz

// Pin input Pull
#define  GPIO_NOPULL                            0x00U
#define  GPIO_PULLUP                            0x01U
#define  GPIO_PULLDOWN                          0x02U

// Pin Output Mode
#define  GPIO_PUSH_PULL                            0x00U
#define  GPIO_OPEN_DRAIN                           0x01U


void GPIO_Pin_Mode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Mode);
void GPIO_Pin_Pull (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Pull);
void GPIO_Pin_Speed (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Speed);
void GPIO_Pin_Set (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_Clear (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_Toggle (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_OutputMode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Output);
void GPIO_Alternate_Function (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t AF);



//temp
void SPI_Test_Init ();
uint8_t SPI_Test_RegisterRead (uint8_t address);

// operate
uint8_t* bufReverse(uint8_t* buf, unsigned int size);




#endif