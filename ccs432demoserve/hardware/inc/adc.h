/****************************************************/
//MSP432P401R
//ADC采集
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/

#ifndef __ADC_H
#define __ADC_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define ADC_TRIGGER_TA0_C1 ADC_TRIGGER_SOURCE1
#define ADC_TRIGGER_TA0_C2 ADC_TRIGGER_SOURCE2
#define ADC_TRIGGER_TA1_C1 ADC_TRIGGER_SOURCE3
#define ADC_TRIGGER_TA1_C2 ADC_TRIGGER_SOURCE4
#define ADC_TRIGGER_TA2_C1 ADC_TRIGGER_SOURCE5
#define ADC_TRIGGER_TA2_C2 ADC_TRIGGER_SOURCE6
#define ADC_TRIGGER_TA3_C1 ADC_TRIGGER_SOURCE7



//引脚
//#define AI_1_CHANL 8
//#define AI_1_PORT GPIO_PORT_P4
//#define AI_1_PIN GPIO_PIN5
//#define AI_1_MEM ADC_MEM8
//#define AI_1_AD ADC_INPUT_A8
//#define AI_1_INT ADC_INT8

#define AI_1_CHANL 0
#define AI_1_PORT GPIO_PORT_P5
#define AI_1_PIN GPIO_PIN5
#define AI_1_MEM ADC_MEM0
#define AI_1_AD ADC_INPUT_A0
#define AI_1_INT ADC_INT0


#define AI_2_CHANL 9
#define AI_2_PORT GPIO_PORT_P4
#define AI_2_PIN GPIO_PIN4
#define AI_2_MEM ADC_MEM9
#define AI_2_AD ADC_INPUT_A9
#define AI_2_INT ADC_INT9

//触发源
#define MY_ADC_TRIGGER_SOURCE ADC_TRIGGER_SOURCE3

//总时间  M*N*21us
#define ADCN 1024 //采样次数
#define ADCM 1   //采样通道个数

extern uint16_t g_u16ADCbuff[ADCM][ADCN];
//ADC转换状态 0 等待 1 正在进行 2完成
extern uint8_t g_flag_ADCStatus;    

void ADC_Config(void);
void adc_dma_init(uint16_t len);
void adc_restart();


#endif
