/*
 * swspi.c
 *
 *  Created on: 2022年7月6日
 *      Author: 14913
 */



#include "swspi.h"




void  SPI_SW_init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN7);
}




void  SPIv_WriteData(uint8_t Data)
{
    uint8_t i=0;
    for(i=8;i>0;i--)
    {
      SPI_SCLK_CLR;
      if(Data&0x80)
          SPI_MOSI_SET;
      else
          SPI_MOSI_CLR;
      SPI_SCLK_SET;
      delay_us(2);
      Data<<=1;
    }
}




















