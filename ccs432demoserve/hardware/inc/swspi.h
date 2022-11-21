/*
 * swspi.h
 *
 *  Created on: 2022年7月6日
 *      Author: 14913
 */

#ifndef HARDWARE_INC_SWSPI_H_
#define HARDWARE_INC_SWSPI_H_

#include "main.h"

#define SPI_CS_SET      GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN7)
#define SPI_MOSI_SET    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN6)
#define SPI_SCLK_SET    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5)

#define SPI_CS_SET      GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN7)
#define SPI_MOSI_CLR    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6)
#define SPI_SCLK_CLR    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5)





void SPI_SW_init(void);
void SPIv_WriteData(uint8_t Data);



#endif /* HARDWARE_INC_SWSPI_H_ */
