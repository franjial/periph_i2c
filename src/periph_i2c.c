#include "periph_i2c.h"

ERRORSTATUS I2C_On(PeriphI2C* this, ERRORCODE* err){

	/* RCC & GPIO */

	if(this->i2c == I2C1){
		RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN_Msk;
		RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;

		GPIOB->MODER   = (GPIOB->MODER & ~(GPIO_MODER_MODER8|GPIO_MODER_MODER9)) | ((GPIO_MODER_MODER8|GPIO_MODER_MODER9) & (uint32_t)(0xA<<16)); /* AF */
		GPIOB->PUPDR   = (GPIOB->PUPDR & ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9)) | ((GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9) & (uint32_t)(0x5<<16)); /* PullUp */
		GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~(GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9)) | ((GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9) & (uint32_t)(0xF<<16)); /* HIGH SPEED */
		GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9); /* PP */
		GPIOB->AFR[1] |= (0x44); /* Select I2C(AF4) alternate function*/
	}
	else if(this->i2c == I2C2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN_Msk;
		/* TODO GPIO */
	}
	else if(this->i2c == I2C3){
		RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN_Msk;
		/* TODO GPIO */
	}
	else{
		/* TODO error code */
		return ERR;
	}


	/**
	 * timming settings -> Standard-mode 10kHz (f_I2CCLK=16MHz)
	 * t_SCLL = 50us
	 * t_SCLH = 49us
	 * t_SCL = ~100us
	 * t_SDADEL = 500ns
	 * t_SCLDEL = 1250ns
	 */

	this->i2c->TIMINGR = (this->i2c->TIMINGR & ~I2C_TIMINGR_PRESC)|(I2C_TIMINGR_PRESC & (0x3<<I2C_TIMINGR_PRESC_Pos));
	this->i2c->TIMINGR = (this->i2c->TIMINGR & ~(I2C_TIMINGR_SCLH|I2C_TIMINGR_SCLL))|((I2C_TIMINGR_SCLH|I2C_TIMINGR_SCLL) & 0xC3C7);
	this->i2c->TIMINGR = (this->i2c->TIMINGR & ~(I2C_TIMINGR_SCLDEL|I2C_TIMINGR_SDADEL))|((I2C_TIMINGR_SCLDEL|I2C_TIMINGR_SDADEL) & (0x42<<I2C_TIMINGR_SDADEL_Pos));

	/* ENABLE */
	this->i2c->CR1 |= I2C_CR1_PE;

	return SUC;
}

ERRORSTATUS I2C_Send(PeriphI2C* this, uint8_t sadd, const char* str, ERRORCODE* err){
	BOOL finish = FALSE;
	int i;

	/* master mode */

	/* address phase */
	this->i2c->CR2 &= ~I2C_CR2_ADD10; /*< 7-bit addressing mode */
	this->i2c->CR2 = (this->i2c->CR2 & ~I2C_CR2_SADD)|(I2C_CR2_SADD & sadd); /*< slave address to be sent */
	this->i2c->CR2 = (this->i2c->CR2 & ~I2C_CR2_NBYTES)|(I2C_CR2_NBYTES & (uint8_t)str[0]); /*< number of bytes to transfer */

	this->i2c->CR2 &= ~I2C_CR2_AUTOEND;
	this->i2c->CR2 |= I2C_CR2_START;

	/* TODO timeout */
	i = 1;
	while(finish==FALSE){
		if((this->i2c->ISR & I2C_ISR_TXIS) != 0){
			this->i2c->TXDR = str[i];
			i++;

			if((this->i2c->CR2 & I2C_CR2_NBYTES) == 0){
				/* NBYTES transmitted */
				if((this->i2c->CR2 & I2C_ISR_TC) == 0){
					finish=TRUE;
				}
				else{
					/* TODO set i2c_cr2.start with slave address NBYTES ... */
					return ERR; /* for now, we don't support to send more than 255 bytes */
				}
			}

		}

		if((this->i2c->ISR & I2C_ISR_NACKF) != 0){
			/* TODO set error code */
			return ERR;
		}

	}

	return SUC;
}

ERRORSTATUS I2C_Recv(PeriphI2C* this, char* str, ERRORCODE* err){
	return SUC;
}
