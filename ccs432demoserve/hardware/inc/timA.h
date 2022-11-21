/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

#ifndef __RNA_TIMA_H
#define __RNA_TIMA_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "main.h"

extern uint8_t TIMA2_CAP_STA;
extern uint16_t TIMA2_CAP_VAL;

extern uint16_t TIM2capdata[10];
extern uint8_t g_flag_cap;

void TimA1_Int_Init(uint16_t ccr0, uint16_t psc);
void TimA0_PWM_Init(uint16_t ccr0, uint16_t psc);
void TimA2_Cap_Init(void);




#endif
