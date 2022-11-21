/****************************************************/
// MSP432P401R
// 串口配置
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/

/******************   版本更新说明   *****************
 * 
 * 支持printf
 * Keil支持标准C库跟微库
 * 
 * ? 需要注意：
 * ①使用标准C库时，将无法使用scanf。
 * 如果需要使用scanf时，请使用微库 MicroLIB
 * ①低频时钟频率下，高波特率使得传输时误差过大,
 * 比如35768Hz下19200波特率,
 * 会使得传输出错，这时可以尝试降低波特率。
 * ②baudrate_calculate的问题请去文件内查看。
 * 
 ****************************************************/

#ifndef __USART_H
#define __USART_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "stdio.h" //1.61328125kb

#ifdef __TI_COMPILER_VERSION__
//CCS平台
#include "stdarg.h"
#include "string.h"
#define USART0_MAX_SEND_LEN     600                 //最大发送缓存字节数
int printf(const char *str, ...);
#endif

void uart_init(uint32_t baudRate);

#endif
