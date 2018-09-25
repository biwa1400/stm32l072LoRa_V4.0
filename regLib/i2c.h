#ifndef __I2C_H
#define __I2C_H

void i2c1_init();

void i2c1_transmit_single_byte (uint8_t addr, uint8_t byte);

void i2c1_receive(uint8_t addr, uint8_t bytes,uint8_t* buf, void (*receiveCallback)(uint8_t* recvBuf));

#endif