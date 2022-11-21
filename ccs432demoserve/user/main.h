/****************************************************/
// MSP432P401R
// 配置Keil独立空工程 (已将ti固件库打包至工程)
// Keil工程已配置开启FPU
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/


#ifndef _MAIN_H
#define _MAIN_H

#include <dac8532dma.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "delay.h"
#include "sysinit.h"
#include "usart.h"
#include "adc.h"
#include "oled.h"
#include "timA.h"
#include "key.h"

#include "dac8532dma.h"
#include "swspi.h"
#include "dac7811.h"
#include "tpl0401.h"


#include "myfft.h"
//#include "lx_DSP.h"


#include "FMAM.h"

void SysInit(void);





extern float duty;
extern uint16_t envbatchdata[1024];
extern uint32_t adc_average;

extern uint8_t testmode ;

#endif /*_MAIN_H*/
