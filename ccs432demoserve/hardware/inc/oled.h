/*
 * @Author: luxun59 68161305+luxun59@users.noreply.github.com
 * @Date: 2022-06-07 22:55:07
 * @LastEditors: luxun59 68161305+luxun59@users.noreply.github.com
 * @LastEditTime: 2022-06-07 23:33:51
 * @FilePath: \新建文件夹 (2)\oled.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __OLED_H
#define __OLED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>

#define HW_IIC 0
#define SW_IIC 1
#define HW_SPI 2
#define SW_SPI 3

#define IIC_1 10
#define IIC_2 11
#define IIC_3 12
#define IIC_4 13

/*****************************************************************/
//配置驱动
//1.HW_IIC  硬件IIC
//2.SW_IIC  软件IIC
//3.HW_SPI	硬件SPI
//4.SW_SPI	软件IIC

//现在只支持 硬件IIC 和 软件IIC
#define TRANSFER_METHOD SW_IIC


//OLED控制用函数
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey);
void OLED_Init(void);
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_Refresh(void);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ScrollDisplay(uint8_t num,uint8_t space);
void OLED_WR_BP(uint8_t x,uint8_t y);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);
void OLED_ShowFloat(uint8_t x,uint8_t y,float f,uint8_t size1);



#endif




