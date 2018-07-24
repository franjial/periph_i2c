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
#define I2C_MODE_MASTER (I2C_MODE)0x00000001
/* TODO continuar con los demas modos */

typedef struct {
	/* configuration: parameters and status */
	FUNCTIONALSTATE fs;
	I2C_TypeDef *i2c;
	I2C_MODE *mode;
}PeriphI2C;

ERRORSTATUS I2C_On(PeriphI2C* this, ERRORCODE* err);
ERRORSTATUS I2C_Off(PeriphI2C* this, ERRORCODE* err);

#endif /* INC_PERIPH_I2C_H_ */
