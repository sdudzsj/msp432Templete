#include <stdint.h>
#include <stdbool.h>


#include "LCD12864.h"


//LCD12864�˿ڶ���

#define LCD12864_CS_PORT GPIO_PORT_P5
#define LCD12864_CS_PIN GPIO_PIN1
#define LCD12864_SID_PORT GPIO_PORT_P3
#define LCD12864_SID_PIN GPIO_PIN5
#define LCD12864_SCLK_PORT GPIO_PORT_P3
#define LCD12864_SCLK_PIN GPIO_PIN7



//
#define LCD12864_CS_HIGH     GPIO_setOutputHighOnPin(LCD12864_CS_PORT, LCD12864_CS_PIN)//Ƭѡ�ź�
#define LCD12864_CS_LOW      GPIO_setOutputLowOnPin(LCD12864_CS_PORT, LCD12864_CS_PIN)//PL0
#define LCD12864_SID_HIGH    GPIO_setOutputHighOnPin(LCD12864_SID_PORT, LCD12864_SID_PIN)//�����ź�
#define LCD12864_SID_LOW     GPIO_setOutputLowOnPin(LCD12864_SID_PORT, LCD12864_SID_PIN)
#define LCD12864_SCLK_HIGH   GPIO_setOutputHighOnPin(LCD12864_SCLK_PORT, LCD12864_SCLK_PIN)//ʱ���ź�
#define LCD12864_SCLK_LOW    GPIO_setOutputLowOnPin(LCD12864_SCLK_PORT, LCD12864_SCLK_PIN)








#if 1


//#define LCD12864_CS_HIGH     GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0)//Ƭѡ�ź�
//#define LCD12864_CS_LOW      GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0)//PL0
//#define LCD12864_SID_HIGH    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1)//�����ź�
//#define LCD12864_SID_LOW     GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0)
//#define LCD12864_SCLK_HIGH   GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2)//ʱ���ź�
//#define LCD12864_SCLK_LOW    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0)
//#define LCD12864_RST   //��λ�ź�
/********************************************************************
* ���� : IO_Init();
* ���� : ��ʼ��io��
* ���� : z
* ��� : ��
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
* ���� : LCD12864_Delay()
* ���� : ��ʱ
* ���� : z
* ��� : ��
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
* ���� : LCD12864_sendbyte
* ���� : ����Һ���Ĵ���ͨ��Э�飬��������
* ���� : zdata
* ��� : ��
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
* ���� : LCD12864_write_com()
* ���� : д����ָ������ֽ�д��
* ���� : com
* ��� : ��
***********************************************************************/
void  LCD12864_WriteCom(uint8_t com)
{
      LCD12864_CS_HIGH;//��Ƭѡ���ߵ�ƽ��Ч
      LCD12864_SendByte(0xf8); //�����һ�ֽ�  ����ʱ��ȷ����  RS RW 0 0
      LCD12864_SendByte(com & 0xf0);     //�ڶ��ֽ�
      LCD12864_SendByte((com << 4) & 0xf0);//�����ֽ�
}

/********************************************************************
* ���� : LCD12864_write_date����
* ���� : д����ָ��
* ���� : date
* ��� : ��
***********************************************************************/
void LCD12864_WriteDate(uint8_t date)
{
    LCD12864_CS_HIGH; //��Ƭѡ
    LCD12864_SendByte(0xfa);//��һ�ֽ�     RS RW  1 0
    LCD12864_SendByte(date & 0xf0);  //�ڶ��ֽ�
    LCD12864_SendByte((date << 4) & 0xf0);//�����ֽ�
}





/********************************************************************
* ���� : LCD12864_init()
* ���� : 12864��ʼ������
* ���� : com
* ��� : ��
***********************************************************************/
void LCD12864_Init()
{
	  IO_Init();
     //LCD12864_RST = 0;
   //  LCD12864_Delay(10);
     //LCD12864_RST = 1;
    // LCD12864_Delay(10);
     //LCD12864_WriteCom(0x34);//����ָ����� ,��ͼ����ʾ
     LCD12864_WriteCom(0x30);//����ָ�����, ��ͼ����ʾ
     LCD12864_WriteCom(0x30);
     LCD12864_WriteCom(0x0C);//����ʾ������ʾ�α���α�λ��
     LCD12864_WriteCom(0x01);//����
     LCD12864_WriteCom(0x06);
  //   LCD12864_Delay(10);

}
void init_picture()//��ʾͼƬ�ĳ�ʼ��������ʹ�ù�������ʾ������ʹ����ʾ���ֳ�ʼ��������

{

   LCD12864_WriteCom(0x36);//дָ���������ָ�����ͼG=1��
   LCD12864_Delay(10);


}
void init_char(){  //��ʾ���ֳ�ʼ������
    LCD12864_WriteCom(0x30);//����ָ�����, ��ͼ����ʾ
 //   LCD12864_WriteCom(0x30);
    LCD12864_WriteCom(0x0C);//����ʾ������ʾ�α���α�λ��
    LCD12864_WriteCom(0x06);

}

void show_Pic(uint8_t *address)//��ʾͼƬ����

{          //address����ָ�������ָ�룬�÷���show_Pic��XY������XYΪ������

   uint8_t i,j;
   init_picture();
   for(i=0;i<32;i++)     //�ϰ�����32��������д��
   {
       LCD12864_WriteCom(0x80+i);//���ʹ�ֱ��ַ
       LCD12864_WriteCom(0x80);  //����ˮƽ��ַ��ˮƽ��ַ���Լ�1
       for(j=0;j<16;j++)//ÿ��128���㣬����һ��Ҫ16����λ16��������Ҳ����8λ��������������ȫ������
       {
          LCD12864_WriteDate(*address);
          address++;
       }
   }
   for(i=0;i<32;i++)    //�°�����32�Ų���ԭ����ϰ���һ��
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
* ���� :LCD_clean_scans()
* ���� : ����
* ���� : ��
* ��� : ��
***********************************************************************/
void LCD12864_CleanScans()
{
     LCD12864_WriteCom(0x01);//����
}
/********************************************************************
* ���� :  LCD12864_pos()
* ���� : �趨��ʾλ��
* ���� :  x,y
* ��� : ��
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
    LCD12864_WriteCom(pos);//��ʾ��ַ
}
/********************************************************************
* ���� : LCD12864_dishz()
* ���� : ��ʾ�ַ���
* ���� : *s
* ��� : ��
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
sbit CS=P2^5;	    //Ƭѡ�ź�
sbit SID=P2^6;		//�����ź�
sbit SCLK=P2^7;		//ʱ���ź� 
sbit RST=P2^2;		//��λ�ź�
sbit CH = P2^4;		//���С�����ѡ���ź�

/********************************************************************
* ���� : delay()
* ���� : ��ʱ,��ʱʱ��Ϊ 100us * t������ͨ�������ʱ����һ����
* ���� : t
* ��� : ��
***********************************************************************/
void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0; i<t;  i++)
    for(j=0; j<10; j++);
}

/********************************************************************
* ���� : sendbyte()
* ���� : ����Һ���Ĵ���ͨ��Э�飬��������
* ���� : zdata
* ��� : ��
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
* ���� : write_com()
* ���� : д����ָ��
* ���� : cmdcode
* ��� : ��
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
* ���� : write_data()
* ���� : д����ָ��
* ���� : cmdcode
* ��� : ��
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
* ���� : lcdinit()
* ���� : ��ʼ������
* ���� : cmdcode
* ��� : ��
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
* ���� : hzkdis()
* ���� : ��ʾ�ַ���
* ���� : *s
* ��� : ��
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
* ���� : Test()
* ���� : ��ʾ�Ӻ���
* ���� : ��
* ��� : ��
***********************************************************************/
void Test()
{  
	write_com(0x03);
	delay(50);    
	write_com(0x81);
	hzkdis("   HOT-51");
	write_com(0x91);
	hzkdis("��Ƭ��������");
	write_com(0x89);
	hzkdis("          ");
	write_com(0x98);
	hzkdis("    ��������");
}


/********************************************************************
* ���� : Main()
* ���� : ������
* ���� : ��
* ��� : ��
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














