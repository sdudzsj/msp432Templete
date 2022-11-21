/*
 * tpl0401.c
 *
 *  Created on: 2022年7月29日
 *      Author: 14913
 */
#include "main.h"


#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //置1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //置0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //获取电平

// I2C SCL  P5.0
#define TPL0401_SCL_PIN_NUM (BIT0)
#define IIC_SCL_IO_INIT (Set_Bit(P5DIR, TPL0401_SCL_PIN_NUM))
#define IIC_SCL_Set() (Set_Bit(P5OUT, TPL0401_SCL_PIN_NUM))
#define IIC_SCL_Clr() (Clr_Bit(P5OUT, TPL0401_SCL_PIN_NUM))

// I2C SDA  P5.2
#define TPL0401_SDA_PIN_NUM (BIT2)
#define IIC_SDA_IO_INIT (Set_Bit(P5DIR, TPL0401_SDA_PIN_NUM))
#define IIC_SDA_Set() (Set_Bit(P5OUT, TPL0401_SDA_PIN_NUM))
#define IIC_SDA_Clr() (Clr_Bit(P5OUT, TPL0401_SDA_PIN_NUM))






void TPL0401_SW_Configuration()
{
  IIC_SCL_IO_INIT;
  IIC_SDA_IO_INIT;
  delay_ms(200);
}

//起始信号
void I2C_Start(void)
{
    IIC_SDA_Set();
    IIC_SCL_Set();
    IIC_SDA_Clr();
    IIC_SCL_Clr();
}

//结束信号
void I2C_Stop(void)
{
    IIC_SDA_Clr();
    IIC_SCL_Set();
    IIC_SDA_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
    IIC_SCL_Clr();
    IIC_SDA_Clr();
    delay_us(2);
    IIC_SCL_Set();
    delay_us(5);
    IIC_SCL_Clr();
}

//写入一个字节
void Send_Byte(uint8_t dat)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    IIC_SCL_Clr(); //将时钟信号设置为低电平
    delay_us(1);
    if (dat & 0x80) //将dat的8位从最高位依次写入
    {
        IIC_SDA_Set();
    }
    else
    {
        IIC_SDA_Clr();
    }

    IIC_SCL_Set();
    delay_us(2);
    IIC_SCL_Clr();
    dat <<= 1;
  }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void TPL0401_WR_Byte(uint8_t dat)
{
  I2C_Start();
  Send_Byte(0x5C);
  I2C_WaitAck();
  Send_Byte(0x00);
  I2C_WaitAck();
  Send_Byte(dat);
  I2C_WaitAck();
  I2C_Stop();
}







