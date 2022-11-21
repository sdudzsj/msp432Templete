#include <stdint.h>
#include <stdbool.h>


#include "LCD12864.h"


//LCD12864端口定义

#define LCD12864_CS_PORT GPIO_PORT_P5
#define LCD12864_CS_PIN GPIO_PIN1
#define LCD12864_SID_PORT GPIO_PORT_P3
#define LCD12864_SID_PIN GPIO_PIN5
#define LCD12864_SCLK_PORT GPIO_PORT_P3
#define LCD12864_SCLK_PIN GPIO_PIN7



//
#define LCD12864_CS_HIGH     GPIO_setOutputHighOnPin(LCD12864_CS_PORT, LCD12864_CS_PIN)//片选信号
#define LCD12864_CS_LOW      GPIO_setOutputLowOnPin(LCD12864_CS_PORT, LCD12864_CS_PIN)//PL0
#define LCD12864_SID_HIGH    GPIO_setOutputHighOnPin(LCD12864_SID_PORT, LCD12864_SID_PIN)//数据信号
#define LCD12864_SID_LOW     GPIO_setOutputLowOnPin(LCD12864_SID_PORT, LCD12864_SID_PIN)
#define LCD12864_SCLK_HIGH   GPIO_setOutputHighOnPin(LCD12864_SCLK_PORT, LCD12864_SCLK_PIN)//时钟信号
#define LCD12864_SCLK_LOW    GPIO_setOutputLowOnPin(LCD12864_SCLK_PORT, LCD12864_SCLK_PIN)








#if 1


//#define LCD12864_CS_HIGH     GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0)//片选信号
//#define LCD12864_CS_LOW      GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0)//PL0
//#define LCD12864_SID_HIGH    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1)//数据信号
//#define LCD12864_SID_LOW     GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0)
//#define LCD12864_SCLK_HIGH   GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2)//时钟信号
//#define LCD12864_SCLK_LOW    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0)
//#define LCD12864_RST   //复位信号
/********************************************************************
* 名称 : IO_Init();
* 功能 : 初始化io口
* 输入 : z
* 输出 : 无
***********************************************************************/
void IO_Init()
{
	  MAP_GPIO_setAsOutputPin(LCD12864_CS_PORT,LCD12864_CS_PIN);
	  MAP_GPIO_setAsOutputPin(LCD12864_SID_PORT,LCD12864_SID_PIN);
	  MAP_GPIO_setAsOutputPin(LCD12864_SCLK_PORT,LCD12864_SCLK_PIN);
	
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
//    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0 | GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
//    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_2|GPIO_PIN_3);
//    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7);
//    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
//	
//		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	
}
/********************************************************************
* 名称 : LCD12864_Delay()
* 功能 : 延时
* 输入 : z
* 输出 : 无
***********************************************************************/
void LCD12864_Delay(uint32_t z)
{
  uint8_t i;
  while(z --)
      for(i = 0;i < 120;i ++);
  //  uint a,b;
  //  for(a=0; a<z; a++)
  //      for(b=0; b<120; b++);
}
/********************************************************************
* 名称 : LCD12864_sendbyte
* 功能 : 按照液晶的串口通信协议，发送数据
* 输入 : zdata
* 输出 : 无
***********************************************************************/
void LCD12864_SendByte(uint8_t zdata)
{
    uint8_t i;
    for(i = 0;i < 8;i ++)
    {
        LCD12864_SCLK_HIGH;
        if((zdata<<i) & 0x80)
        {
            LCD12864_SID_HIGH;
        }
        else
        {
            LCD12864_SID_LOW;
        }
        LCD12864_SCLK_LOW;
				delay_us(100);
				//LCD12864_Delay(100);
        //SysCtlDelay(1000);
    }
}
/********************************************************************
* 名称 : LCD12864_write_com()
* 功能 : 写串口指令，分三字节写完
* 输入 : com
* 输出 : 无
***********************************************************************/
void  LCD12864_WriteCom(uint8_t com)
{
      LCD12864_CS_HIGH;//打开片选，高电平有效
      LCD12864_SendByte(0xf8); //送入第一字节  根据时序确定的  RS RW 0 0
      LCD12864_SendByte(com & 0xf0);     //第二字节
      LCD12864_SendByte((com << 4) & 0xf0);//第三字节
}

/********************************************************************
* 名称 : LCD12864_write_date（）
* 功能 : 写串口指令
* 输入 : date
* 输出 : 无
***********************************************************************/
void LCD12864_WriteDate(uint8_t date)
{
    LCD12864_CS_HIGH; //打开片选
    LCD12864_SendByte(0xfa);//第一字节     RS RW  1 0
    LCD12864_SendByte(date & 0xf0);  //第二字节
    LCD12864_SendByte((date << 4) & 0xf0);//第三字节
}





/********************************************************************
* 名称 : LCD12864_init()
* 功能 : 12864初始化函数
* 输入 : com
* 输出 : 无
***********************************************************************/
void LCD12864_Init()
{
	  IO_Init();
     //LCD12864_RST = 0;
   //  LCD12864_Delay(10);
     //LCD12864_RST = 1;
    // LCD12864_Delay(10);
     //LCD12864_WriteCom(0x34);//扩充指令集动作 ,绘图不显示
     LCD12864_WriteCom(0x30);//基本指令集动作, 绘图不显示
     LCD12864_WriteCom(0x30);
     LCD12864_WriteCom(0x0C);//开显示，不显示游标和游标位置
     LCD12864_WriteCom(0x01);//清屏
     LCD12864_WriteCom(0x06);
  //   LCD12864_Delay(10);

}
void init_picture()//显示图片的初始化函数（使用过后再显示文字需使用显示文字初始化函数）

{

   LCD12864_WriteCom(0x36);//写指令函数，扩充指令集，绘图G=1打开
   LCD12864_Delay(10);


}
void init_char(){  //显示文字初始化函数
    LCD12864_WriteCom(0x30);//基本指令集动作, 绘图不显示
 //   LCD12864_WriteCom(0x30);
    LCD12864_WriteCom(0x0C);//开显示，不显示游标和游标位置
    LCD12864_WriteCom(0x06);

}

void show_Pic(uint8_t *address)//显示图片函数

{          //address是是指向数组的指针，用法：show_Pic（XY）当中XY为数组名

   uint8_t i,j;
   init_picture();
   for(i=0;i<32;i++)     //上半屏的32排依次先写满
   {
       LCD12864_WriteCom(0x80+i);//先送垂直地址
       LCD12864_WriteCom(0x80);  //再送水平地址，水平地址可自加1
       for(j=0;j<16;j++)//每排128个点，所以一共要16个两位16进制数（也就是8位二进制数）才能全部控制
       {
          LCD12864_WriteDate(*address);
          address++;
       }
   }
   for(i=0;i<32;i++)    //下半屏的32排操作原理和上半屏一样
   {
      LCD12864_WriteCom(0x80+i);
      LCD12864_WriteCom(0x88);
      for(j=0;j<16;j++)
       {
          LCD12864_WriteDate(*address);
          address++;
       }
   }
   init_char();
}
/*void show_Pic_H(uint8_t *address){
  uint8_t i,j;
  for(i=0;i<64;i++){
    for(j=0;j<32;j++){
      LCD12864_WriteCom(0x80+i);
      LCD12864_WriteCom(0x80+j);
      LCD12864_WriteDate(*address);
      address++;
    }
  }
}*/
/********************************************************************
* 名称 :LCD_clean_scans()
* 功能 : 清屏
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LCD12864_CleanScans()
{
     LCD12864_WriteCom(0x01);//清屏
}
/********************************************************************
* 名称 :  LCD12864_pos()
* 功能 : 设定显示位置
* 输入 :  x,y
* 输出 : 无
***********************************************************************/
void LCD12864_SetPos(uint8_t x,uint8_t y)
{
    uint8_t pos;
    if(x == 0)
            x = 0x80;
    else if(x == 1)
            x = 0x90;
    else if(x == 2)
            x = 0x88;
    else if(x == 3)
            x = 0x98;
    pos = x + y;
    LCD12864_WriteCom(pos);//显示地址
}
/********************************************************************
* 名称 : LCD12864_dishz()
* 功能 : 显示字符串
* 输入 : *s
* 输出 : 无
***********************************************************************/
void LCD12864_DisplayString(uint8_t *s,uint8_t n)
{
    uint8_t  i = 0,mun=0;
    mun=n;
    for(i=0;i<mun;i++)
    {
        LCD12864_WriteDate(s[i]);

        LCD12864_Delay(5);
    }
}


void set_pos(uint8_t addr,uint8_t x)
{
     x&=0x07;
     LCD12864_WriteCom(addr+x);
}
void lcd_send_data(uint8_t x,uint8_t y,uint8_t *s)
{
     LCD12864_SetPos(x,y);
     while(*s!=0){
          LCD12864_WriteDate(*s);
          LCD12864_Delay(5);
          s++;
     }

}
#endif

#if 0
sbit CS=P2^5;	    //片选信号
sbit SID=P2^6;		//数据信号
sbit SCLK=P2^7;		//时钟信号 
sbit RST=P2^2;		//复位信号
sbit CH = P2^4;		//并行、串行选择信号

/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间为 100us * t。这是通过软件延时，有一定误差。
* 输入 : t
* 输出 : 无
***********************************************************************/
void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0; i<t;  i++)
    for(j=0; j<10; j++);
}

/********************************************************************
* 名称 : sendbyte()
* 功能 : 按照液晶的串口通信协议，发送数据
* 输入 : zdata
* 输出 : 无
***********************************************************************/
void sendbyte(unsigned char zdata)
{
	unsigned int i;
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			SID = 1;
		}
		else 
		{
			SID = 0;
		}
		SCLK = 0;
		SCLK = 1;
	}
}

/********************************************************************
* 名称 : write_com()
* 功能 : 写串口指令
* 输入 : cmdcode
* 输出 : 无
***********************************************************************/
void write_com(unsigned char cmdcode)
{
	CS = 1;
	sendbyte(0xf8);
	sendbyte(cmdcode & 0xf0);
	sendbyte((cmdcode << 4) & 0xf0);
	delay(2);
}

/********************************************************************
* 名称 : write_data()
* 功能 : 写串口指令
* 输入 : cmdcode
* 输出 : 无
***********************************************************************/
void write_data(unsigned char Dispdata)
{
	CS = 1;
	sendbyte(0xfa);
	sendbyte(Dispdata & 0xf0);
	sendbyte((Dispdata << 4) & 0xf0);
	delay(2);
}

/********************************************************************
* 名称 : lcdinit()
* 功能 : 初始化函数
* 输入 : cmdcode
* 输出 : 无
***********************************************************************/
void lcdinit()
{  
	RST = 0;
	delay(100);
	RST = 1;
	delay(20000);
	write_com(0x30);
	delay(50);
	write_com(0x0c);
	delay(50);
}

/********************************************************************
* 名称 : hzkdis()
* 功能 : 显示字符串
* 输入 : *s
* 输出 : 无
***********************************************************************/
void hzkdis(unsigned char code *s)
{  
	while(*s > 0)
    { 
		write_data(*s);
		s++;
		delay(50);
    }
}

/********************************************************************
* 名称 : Test()
* 功能 : 显示子函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Test()
{  
	write_com(0x03);
	delay(50);    
	write_com(0x81);
	hzkdis("   HOT-51");
	write_com(0x91);
	hzkdis("单片机开发板");
	write_com(0x89);
	hzkdis("          ");
	write_com(0x98);
	hzkdis("    东流电子");
}


/********************************************************************
* 名称 : Main()
* 功能 : 主函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Main()
{ 
	CH = 0;
	delay(1);
	lcdinit();
	delay(10);
	while(1)
	{
		Test();
		delay(5000);
	}
}

#endif














