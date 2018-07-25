/*
 * periph_i2c.h
 *
 *  Created on: 22 jul. 2018
 *      Author: francisco
 */

#ifndef INC_PERIPH_I2C_H_
#define INC_PERIPH_I2C_H_

#include <mistipos.h>
#include <stm32l4xx.h>

#define I2C_MODE uint32_t
#define I2C_MODE_S_RECEIVER    (I2C_MODE)0x00000000
#define I2C_MODE_S_TRANSMITTER (I2C_MODE)0x00000100
#define I2C_MODE_M_RECEIVER    (I2C_MODE)0x00000001
#define I2C_MODE_M_TRANSMITTER (I2C_MODE)0x00000101
#define I2C_MODE_M             (uint8_t)0x01
#define I2C_MODE_S             (uint8_t)0x00
/* TODO continuar con los demas modos */

typedef struct {
	/* configuration: parameters and status */
	FUNCTIONALSTATE fs;
	I2C_TypeDef    *i2c;
	I2C_MODE       *mode;

	uint8_t        oar1;        /*< (slave mode) 8-bit slave address  */
	uint8_t        oar2;        /*< (slave mode) 8-bit slave address  */
	uint8_t        sadd;        /*< (master mode) 8-bit slave address */

}PeriphI2C;


ERRORSTATUS I2C_On(PeriphI2C* this, ERRORCODE* err);
ERRORSTATUS I2C_Off(PeriphI2C* this, ERRORCODE* err);
ERRORSTATUS I2C_Send(PeriphI2C* this, uint8_t sadd, const char* str, ERRORCODE* err);
ERRORSTATUS I2C_Recv(PeriphI2C* this, char* str, ERRORCODE* err);

#endif /* INC_PERIPH_I2C_H_ */
