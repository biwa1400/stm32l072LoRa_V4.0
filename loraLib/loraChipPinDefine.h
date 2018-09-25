#ifndef __loraChipPinDefine_H__
#define __loraChipPinDefine_H__

#define RADIO_SPI SPI1

#define RADIO_TCXO_VCC_PORT  GPIOB
#define RADIO_TCXO_VCC_PIN   5
#define RADIO_TCXO_VCC_MODE  GPIO_MODE_OUTPUT_PP
#define RADIO_TCXO_VCC_PULL  GPIO_PULLUP
#define RADIO_TCXO_VCC_SPEED GPIO_SPEED_FREQ_LOW

#define RADIO_RESET_PORT  GPIOC
#define RADIO_RESET_PIN   0
#define RADIO_RESET_MODE  GPIO_MODE_OUTPUT_PP
#define RADIO_RESET_PULL  GPIO_NOPULL
#define RADIO_RESET_SPEED GPIO_SPEED_FREQ_LOW


#define RADIO_MOSI_PORT   GPIOA
#define RADIO_MOSI_PIN    7
#define RADIO_MOSI_MODE   GPIO_MODE_AF_PP
#define RADIO_MOSI_AF     0
#define RADIO_MOSI_PULL   GPIO_PULLDOWN
#define RADIO_MOSI_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_MISO_PORT   GPIOA
#define RADIO_MISO_PIN    6
#define RADIO_MISO_MODE   GPIO_MODE_AF_PP
#define RADIO_MISO_AF     0
#define RADIO_MISO_PULL   GPIO_PULLDOWN
#define RADIO_MISO_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_SCLK_PORT   GPIOB
#define RADIO_SCLK_PIN    3
#define RADIO_SCLK_MODE   GPIO_MODE_AF_PP
#define RADIO_SCLK_AF     0
#define RADIO_SCLK_PULL   GPIO_PULLDOWN
#define RADIO_SCLK_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_NSS_PORT    GPIOA
#define RADIO_NSS_PIN     15
#define RADIO_NSS_MODE    GPIO_MODE_AF_PP
#define RADIO_NSS_AF      0
#define RADIO_NSS_PULL    GPIO_PULLUP
#define RADIO_NSS_SPEED   GPIO_SPEED_FREQ_HIGH



#define RADIO_ANT_SWITCH_RX_PORT        GPIOA
#define RADIO_ANT_SWITCH_RX_PIN         1
#define RADIO_ANT_SWITCH_RX_MODE        GPIO_MODE_OUTPUT_PP
#define RADIO_ANT_SWITCH_RX_SPEED       GPIO_SPEED_FREQ_LOW
#define RADIO_ANT_SWITCH_RX_PULL        GPIO_NOPULL

#define RADIO_ANT_SWITCH_TX_BOOST_PORT  GPIOC
#define RADIO_ANT_SWITCH_TX_BOOST_PIN   1
#define RADIO_ANT_SWITCH_TX_BOOST_MODE  GPIO_MODE_OUTPUT_PP
#define RADIO_ANT_SWITCH_TX_BOOST_SPEED GPIO_SPEED_FREQ_LOW
#define RADIO_ANT_SWITCH_TX_BOOST_PULL  GPIO_NOPULL

#define RADIO_ANT_SWITCH_TX_RFO_PORT    GPIOC
#define RADIO_ANT_SWITCH_TX_RFO_PIN     2
#define RADIO_ANT_SWITCH_TX_RFO_MODE    GPIO_MODE_OUTPUT_PP
#define RADIO_ANT_SWITCH_TX_RFO_SPEED   GPIO_SPEED_FREQ_LOW
#define RADIO_ANT_SWITCH_TX_RFO_PULL    GPIO_NOPULL

#define RADIO_DIO0_PORT GPIOB
#define RADIO_DIO0_PIN  4
#define RADIO_DIO0_PIN_MODE    GPIO_MODE_INPUT
#define RADIO_DIO0_PIN_SPEED   GPIO_SPEED_FREQ_LOW
#define RADIO_DIO0_PIN_PULL    GPIO_NOPULL

#define RADIO_DIO1_PORT GPIOB
#define RADIO_DIO1_PIN  1
#define RADIO_DIO1_PIN_MODE    GPIO_MODE_INPUT
#define RADIO_DIO1_PIN_SPEED   GPIO_SPEED_FREQ_LOW
#define RADIO_DIO1_PIN_PULL    GPIO_NOPULL

#define RADIO_DIO2_PORT GPIOB
#define RADIO_DIO2_PIN  0
#define RADIO_DIO2_PIN_MODE    GPIO_MODE_INPUT
#define RADIO_DIO2_PIN_SPEED   GPIO_SPEED_FREQ_LOW
#define RADIO_DIO2_PIN_PULL    GPIO_NOPULL

#define RADIO_DIO3_PORT GPIOC
#define RADIO_DIO3_PIN  13
#define RADIO_DIO3_PIN_MODE    GPIO_MODE_INPUT
#define RADIO_DIO3_PIN_SPEED   GPIO_SPEED_FREQ_LOW
#define RADIO_DIO3_PIN_PULL    GPIO_NOPULL

#define RADIO_RCC_IOPENR (RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN | RCC_IOPENR_IOPCEN)

#define RADIO_DIO_0_INTERRUPT_PRIORITY  1
#define RADIO_DIO_1_INTERRUPT_PRIORITY  1

#endif