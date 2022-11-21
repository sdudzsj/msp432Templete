/****************************************************/
//MSP432P401R
//板载按键驱动
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "main.h"

#define KEY1 BITBAND_PERI(P1IN, 1) //读取按键1
#define KEY2 BITBAND_PERI(P1IN, 4) //读取按键2

#define KEY3 BITBAND_PERI(P3IN, 2) //读取按键3
#define KEY4 BITBAND_PERI(P3IN, 3) //读取按键4
#define KEY5 BITBAND_PERI(P6IN, 0) //读取按键5



#define KEY1_PRES 1 //KEY1按下
#define KEY2_PRES 2 //KEY2按下

#define KEY3_PRES 3 //KEY3按下
#define KEY4_PRES 4 //KEY4按下
#define KEY5_PRES 5 //KEY5按下




#define KEY_MODE_INT 1
#define KEY_MODE_SCAN 0

void Usr_Key1_Callback();
void Usr_Key2_Callback();
void Usr_Key3_Callback();
void Usr_Key4_Callback();
void Usr_Key5_Callback();


void KEY_Init(bool mode);    //IO初始化
uint8_t KEY_Scan(bool mode); //按键扫描函数

#endif
