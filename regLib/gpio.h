#ifndef __GPIO_H
#define __GPIO_H

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



/*     |   AF0  |   AF1  |   AF2  |   AF3  |   AF4  |   AF5  |   AF6  |   AF7  |
 *______________________________________________________________________________
 * PA0 |        |        |TIM2_C1 |TSC     |USART2  |TIM2_TR |USART4  |COMP1   |
 * PA1 |EVENTOUT|LCD     |TIM2_C2 |TSC     |USART2  |TIM21_TR|USART4  |        |
 * PA2 |TIM21_C1|LCD     |TIM2_C3 |TSC     |USART2  |        |LPUART1 |COMP2   |
 * PA3 |TIM21_C2|LCD     |TIM2_C4 |TSC     |USART2  |        |LPUART1 |        |
 * PA4 |SPI1    |        |        |TSC     |USART2  |TIM22_TR|        |        |
 * PA5 |SPI1    |        |TIM2_TR |TSC     |        |TIM2_C1 |        |        |
 * PA6 |SPI1    |LCD     |TIM3_C1 |TSC     |LPUART1 |TIM22_C1|EVENTOUT|COMP1   |
 * PA7 |SPI1    |LCD     |TIM3_C2 |TSC     |        |TIM22_C2|EVENTOUT|COMP2   |
 * PA8 |MCO     |LCD     |USB     |EVENTOUT|USART1  |        |        |I2C3    |
 * PA9 |MCO     |LCD     |        |TSC     |USART1  |        |I2C1    |I2C3    |
 * PA10|        |LCD     |        |TSC     |USART1  |        |I2C1    |        |
 * PA11|SPI1    |        |EVENTOUT|TSC     |USART1  |        |I2C2    |COMP1   |
 * PA12|SPI1    |        |EVENTOUT|TSC     |USART1  |        |I2C2    |COMP2   |
 * PA13|SWDIO   |        |USB     |        |        |        |LPUART1 |        |
 * PA14|SWCLK   |        |        |        |USART2  |        |LPUART1 |        |
 * PA15|SPI1    |LCD     |TIM2_TR |EVENTOUT|USART2  |TIM2_C1 |USART4  |        |
 *______________________________________________________________________________
 * PB0 |EVENTOUT|LCD     |TIM3_C3 |TSC     |        |        |        |        |
 * PB1 |        |LCD     |TIM3_C4 |TSC     |LPUART1 |        |        |        |
 * PB2 |        |        |LPTIM1_O|TSC     |        |        |        |I2C3    |
 * PB3 |SPI1    |LCD     |TIM2_C2 |TSC     |EVENTOUT|USART1  |USART5  |        |
 * PB4 |SPI1    |LCD     |TIM3_C1 |TSC     |TIM22_C1|USART1  |USART5  |I2C3    |
 * PB5 |SPI1    |LCD     |LPTIM1_I|I2C1    |TIM3_C2 |USART1  |USART5  |        |
 *     |        |        |        |        |TIM22_C2|        |        |        |
 * PB6 |USART1  |I2C1    |LPTIM1_T|TSC     |        |        |        |        |
 * PB7 |USART1  |I2C1    |LPTIM1_I|TSC     |        |        |USART4  |        |
 * PB8 |        |LCD     |        |TSC     |I2C1    |        |        |        |
 * PB9 |        |LCD     |EVENTOUT|        |I2C1    |SPI2    |        |        |
 * PB10|        |LCD     |TIM2_C3 |TSC     |LPUART1 |SPI2    |I2C2    |LPUART1 |
 * PB11|EVENTOUT|LCD     |TIM2_C4 |TSC     |LPUART1 |        |I2C2    |LPUART1 |
 * PB12|SPI2    |LCD     |LPUART1 |TSC     |        |I2C2    |EVENTOUT|        |
 * PB13|SPI2    |LCD     |MCO     |TSC     |LPUART1 |I2C2    |TIM21_C1|        |
 * PB14|SPI2    |LCD     |RTC     |TSC     |LPUART1 |I2C2    |TIM21_C2|        |
 * PB15|SPI2    |LCD     |RTC     |        |        |        |        |        |
 *______________________________________________________________________________
 * PC0 |LPTIM1_I|LCD     |EVENTOUT|TSC     |        |        |LPUART1 |I2C3    |
 * PC1 |LPTIM1_O|LCD     |EVENTOUT|TSC     |        |        |LPUART1 |I2C3    |
 * PC2 |LPTIM1_I|LCD     |SPI2    |TSC     |        |        |        |        |
 * PC3 |LPTIM1_T|LCD     |SPI2    |TSC     |        |        |        |        |
 * PC4 |EVENTOUT|LCD     |LPUART1 |        |        |        |        |        |
 * PC5 |        |LCD     |LPUART1 |TSC     |        |        |        |        |
 * PC6 |TIM22_C1|LCD     |TIM3_C1 |TSC     |        |        |        |        |
 * PC7 |TIM22_C2|LCD     |TIM3_C2 |TSC     |        |        |        |        |
 * PC8 |TIM22_TR|LCD     |TIM3_C3 |TSC     |        |        |        |        |
 * PC9 |TIM21_TR|LCD     |USB     |TSC     |        |        |        |I2C3    |
 *     |        |        |TIM3_C4 |        |        |        |        |        |
 * PC10|LPUART1 |LCD     |        |        |        |        |USART4  |        |
 * PC11|LPUART1 |LCD     |        |        |        |        |USART4  |        |
 * PC12|        |LCD     |USART5  |        |        |        |USART4  |        |
 * PC13|        |        |        |        |        |        |        |        |
 * PC14|        |        |        |        |        |        |        |        |
 * PC15|        |        |        |        |        |        |        |        |
 *______________________________________________________________________________
 * PD0 |TIM21_C1|SPI2    |        |        |        |        |        |        |
 * PD1 |        |SPI2    |        |        |        |        |        |        |
 * PD2 |LPUART1 |LCD     |TIM3_TR |        |        |        |USART5  |        |
 * PD3 |USART2  |LCD     |SPI2    |        |        |        |        |        |
 * PD4 |USART2  |SPI2    |        |        |        |        |        |        |
 * PD5 |USART2  |        |        |        |        |        |        |        |
 * PD6 |USART2  |        |        |        |        |        |        |        |
 * PD7 |USART2  |TIM21_C2|        |        |        |        |        |        |
 * PD8 |LPUART1 |LCD     |        |        |        |        |        |        |
 * PD9 |LPUART1 |LCD     |        |        |        |        |        |        |
 * PD10|        |LCD     |        |        |        |        |        |        |
 * PD11|LPUART1 |LCD     |        |        |        |        |        |        |
 * PD12|LPUART1 |LCD     |        |        |        |        |        |        |
 * PD13|        |LCD     |        |        |        |        |        |        |
 * PD14|        |LCD     |        |        |        |        |        |        |
 * PD15|USB     |LCD     |        |        |        |        |        |        |
 *______________________________________________________________________________
 * PE0 |        |LCD     |EVENTOUT|        |        |        |        |        |
 * PE1 |        |LCD     |EVENTOUT|        |        |        |        |        |
 * PE2 |        |LCD     |TIM3_TR |        |        |        |        |        |
 * PE3 |TIM22_C1|LCD     |TIM3_C1 |        |        |        |        |        |
 * PE4 |TIM22_C2|        |TIM3_C2 |        |        |        |        |        |
 * PE5 |TIM21_C1|        |TIM3_C3 |        |        |        |        |        |
 * PE6 |TIM21_C2|        |TIM3_C4 |        |        |        |        |        |
 * PE7 |        |LCD     |        |        |        |        |USART5  |        |
 * PE8 |        |LCD     |        |        |        |        |USART4  |        |
 * PE9 |TIM2_C1 |LCD     |TIM2_TR |        |        |        |USART4  |        |
 * PE10|TIM2_C2 |LCD     |        |        |        |        |USART5  |        |
 * PE11|TIM2_C3 |        |        |        |        |        |USART5  |        |
 * PE12|TIM2_C4 |        |SPI1    |        |        |        |        |        |
 * PE13|        |LCD     |SPI1    |        |        |        |        |        |
 * PE14|        |LCD     |SPI1    |        |        |        |        |        |
 * PE15|        |LCD     |SPI1    |        |        |        |        |        |
 *______________________________________________________________________________
 * PH0 |USB     |        |        |        |        |        |        |        |
 * PH1 |        |        |        |        |        |        |        |        |
 * PH9 |        |        |        |        |        |        |        |        |
 * PH10|        |        |        |        |        |        |        |        |
 *
 */

/** @defgroup GPIOEx_Alternate_function_selection Alternate function selection
 * @{
 */

/*
 * Alternate function AF0
 */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00U)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_TIM21         ((uint8_t)0x00U)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF0_SPI1          ((uint8_t)0x00U)  /* SPI1 Alternate Function mapping     */
#define GPIO_AF0_MCO           ((uint8_t)0x00U)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00U)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00U)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00U)  /* USART1 Alternate Function mapping   */
#define GPIO_AF0_SPI2          ((uint8_t)0x00U)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF0_LPTIM1        ((uint8_t)0x00U)  /* LPTIM1 Alternate Function mapping   */
#define GPIO_AF0_TIM22         ((uint8_t)0x00U)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF0_LPUART1       ((uint8_t)0x00U)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF0_USART2        ((uint8_t)0x00U)  /* USART2 Alternate Function mapping   */
#define GPIO_AF0_TIM2          ((uint8_t)0x00U)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF0_USB           ((uint8_t)0x00U)  /* USB Alternate Function mapping      */
/**
 *
 */

/*
 * Alternate function AF1
 */
#define GPIO_AF1_I2C1          ((uint8_t)0x01U)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF1_SPI2          ((uint8_t)0x01U)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF1_TIM21         ((uint8_t)0x01U)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF1_LCD           ((uint8_t)0x01U)  /* LCD Alternate Function mapping      */
/**
 *
 */

/*
 * Alternate function AF2
 */
#define GPIO_AF2_TIM2          ((uint8_t)0x02U)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF2_TIM3          ((uint8_t)0x02U)  /* TIM3 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT      ((uint8_t)0x02U)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF2_LPTIM1        ((uint8_t)0x02U)  /* LPTIM1 Alternate Function mapping   */
#define GPIO_AF2_LPUART1       ((uint8_t)0x02U)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF2_MCO           ((uint8_t)0x02U)  /* MCO Alternate Function mapping      */
#define GPIO_AF2_RTC           ((uint8_t)0x02U)  /* RTC Alternate Function mapping      */
#define GPIO_AF2_SPI2          ((uint8_t)0x02U)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF2_USART5        ((uint8_t)0x02U)  /* USART5 Alternate Function mapping   */
#define GPIO_AF2_SPI1          ((uint8_t)0x02U)  /* SPI1 Alternate Function mapping     */
#define GPIO_AF2_USB           ((uint8_t)0x00U)  /* USB Alternate Function mapping      */
/**
 *
 */

/*
 * Alternate function AF3
 */
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03U)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF3_I2C1          ((uint8_t)0x03U)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF3_TSC           ((uint8_t)0x03U)  /* TSC  Alternate Function mapping     */
/**
 *
 */

/*
 * Alternate function AF4
 */
#define GPIO_AF4_USART2          ((uint8_t)0x04U)  /* USART2 Alternate Function mapping  */
#define GPIO_AF4_LPUART1         ((uint8_t)0x04U)  /* LPUART1 Alternate Function mapping */
#define GPIO_AF4_USART1          ((uint8_t)0x04U)  /* USART1 Alternate Function mapping  */
#define GPIO_AF4_EVENTOUT        ((uint8_t)0x04U)  /* EVENTOUT Alternate Function mapping*/
#define GPIO_AF4_TIM22           ((uint8_t)0x04U)  /* TIM22 Alternate Function mapping   */
#define GPIO_AF4_TIM3            ((uint8_t)0x04U)  /* TIM3 Alternate Function mapping    */
#define GPIO_AF4_I2C1            ((uint8_t)0x04U)  /* I2C1 Alternate Function mapping    */
/**
 *
 */

/*
 * Alternate function AF5
 */
#define GPIO_AF5_TIM2          ((uint8_t)0x05U)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF5_TIM21         ((uint8_t)0x05U)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF5_TIM22         ((uint8_t)0x05U)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF5_USART1        ((uint8_t)0x05U)  /* USART1 Alternate Function mapping   */
#define GPIO_AF5_SPI2          ((uint8_t)0x05U)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF5_I2C2          ((uint8_t)0x05U)  /* I2C2 Alternate Function mapping     */
/**
 *
 */

/*
 * Alternate function AF6
 */

#define GPIO_AF6_USART4        ((uint8_t)0x06U)  /* USART4 Alternate Function mapping   */
#define GPIO_AF6_LPUART1       ((uint8_t)0x06U)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF6_EVENTOUT      ((uint8_t)0x06U)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF6_I2C1          ((uint8_t)0x06U)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF6_I2C2          ((uint8_t)0x06U)  /* I2C2 Alternate Function mapping     */
#define GPIO_AF6_USART5        ((uint8_t)0x06U)  /* USART5 Alternate Function mapping   */
#define GPIO_AF6_TIM21         ((uint8_t)0x06U)  /* TIM21 Alternate Function mapping    */
/**
 *
 */

/*
 * Alternate function AF7
 */
#define GPIO_AF7_COMP1        ((uint8_t)0x07U)  /* COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07U)  /* COMP2 Alternate Function mapping     */
#define GPIO_AF7_I2C3         ((uint8_t)0x07U)  /* I2C3 Alternate Function mapping      */
#define GPIO_AF7_LPUART1      ((uint8_t)0x07U)  /* LPUART1 Alternate Function mapping   */


void GPIO_Pin_Mode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Mode);
void GPIO_Pin_Pull (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Pull);
void GPIO_Pin_Speed (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Speed);
void GPIO_Pin_Set (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_Clear (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_Toggle (GPIO_TypeDef * GPIOx, uint32_t Pin);
void GPIO_Pin_OutputMode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Output);
void GPIO_Alternate_Function (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t AF);

#endif