/*
 * myspi.h
 *
 *  Created on: 2022年7月5日
 *      Author: 14913
 */

#ifndef DAC8532DMA_H_
#define DAC8532DMA_H_



#include "main.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


//分别写入AB
#define LOAD_NEWDATA_TO_DACA 0x10
#define LOAD_NEWDATA_TO_DACB 0x20
//同时输出需下面两步
#define DATA_TO_DATABUFFERA 0x00
#define DATA_TO_DATABUFFERB_AND_LOAD_DAC_AB 0x34
//通道断电
#define POWERDOWN_DACA_TO_HIGH 0x13
#define POWERDOWN_DACB_TO_HIGH 0x23


#define  MAP_SPI_MSG_LENGTH    3
#define  DACN  128


extern uint8_t g_u8dac8532txData[DACN][3];
extern uint8_t g_flag_DAC8532Status;


void dac8532_start(void);









#endif /* DAC8532DMA_H_ */
