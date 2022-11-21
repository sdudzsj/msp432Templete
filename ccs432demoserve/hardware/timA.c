/****************************************************/
// MSP432P401R
// 定时器A
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/

#include "timA.h"

/**************************************         TIMA2          *******************************************/

#define CAP_TIMA_SELECTION TIMER_A2_BASE                         //在这里改定时器
#define CAP_REGISTER_SELECTION TIMER_A_CAPTURECOMPARE_REGISTER_1 //在这里改定时器通道
#define CAP_CCR_NUM 1                                            //在这里改定时器通道
#define CAP_PORT_PIN GPIO_PORT_P5, GPIO_PIN6                     //在这里改复用引脚


//void DAC8532timer_callback(void);




void TimA2_Cap_Init(void)
{
    // 1.复用输出
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(CAP_PORT_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    /* 定时器配置参数*/
    Timer_A_ContinuousModeConfig continuousModeConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,      // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_48, // SMCLK/48 = 1MHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,  // 开启定时器溢出中断
        TIMER_A_DO_CLEAR                // Clear Counter
    };
    // 3.将定时器初始化为连续计数模式
    MAP_Timer_A_configureContinuousMode(CAP_TIMA_SELECTION, &continuousModeConfig);

    // 4.配置捕捉模式结构体 */
    const Timer_A_CaptureModeConfig captureModeConfig_TA2 = {
        CAP_REGISTER_SELECTION,                      //在这里改引脚
        TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE, //上升下降沿捕获
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,           //CCIxA:外部引脚输入  （CCIxB:与内部ACLK连接(手册)
        TIMER_A_CAPTURE_SYNCHRONOUS,                 //同步捕获
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,     //开启CCRN捕获中断
        TIMER_A_OUTPUTMODE_OUTBITVALUE               //输出位值
    };
    // 5.初始化定时器的捕获模式
    MAP_Timer_A_initCapture(CAP_TIMA_SELECTION, &captureModeConfig_TA2);

    // 6.选择连续模式计数开始计数
    MAP_Timer_A_startCounter(CAP_TIMA_SELECTION, TIMER_A_CONTINUOUS_MODE);

    // 7.清除中断标志位
    MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION);                                   //清除定时器溢出中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION); //清除 CCR1 更新中断标志位

    // 8.开启定时器端口中断
    MAP_Interrupt_enableInterrupt(INT_TA2_N); //开启定时器A2端口中断
}
// 10.编写TIMA ISR ↓↓↓↓


uint16_t TIM2capdata[10];
uint8_t g_flag_cap=0;

void TA2_N_IRQHandler(void)
{
    static uint16_t index = 0;
    g_flag_cap =1;

    if (MAP_Timer_A_getEnabledInterruptStatus(CAP_TIMA_SELECTION)) //溢出中断
    {
       MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION); //清除定时器溢出中断标志位
        /* ★ 软件复位COV ★ */
       /* 这里UP忘记讲了，如果在未清除中断位值时，来了一次中断，COV会置位，需要软件复位，这里没有官方库函数。具体可以参考技术手册(slau356h.pdf) P790 */
       BITBAND_PERI(TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCTL[CAP_CCR_NUM], TIMER_A_CCTLN_COV_OFS) = 0;

        MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION); //清除定时器溢出中断标志位
    }
    if (MAP_Timer_A_getCaptureCompareEnabledInterruptStatus(CAP_TIMA_SELECTION, TIMER_A_CAPTURECOMPARE_REGISTER_1)) //捕获中断
    {
        MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, TIMER_A_CAPTURECOMPARE_REGISTER_1); //清除 CCR1 更新中断标志位
        TIM2capdata[index] = Timer_A_getCaptureCompareCount(CAP_TIMA_SELECTION, TIMER_A_CAPTURECOMPARE_REGISTER_1);
        index++;
        if(index>=10)
        {
            Timer_A_stopTimer(TIMER_A2_BASE);
            g_flag_cap = 2;
            index = 0;
        }
    }

}
/*********************************************************************************************************/

/**************************************         TIMA1          *******************************************/

void TimA1_Int_Init(uint16_t ccr0, uint16_t psc)
{
	  /* Timer_A Continuous Mode Configuration Parameter */
		const Timer_A_UpModeConfig upModeConfig =
		{
						TIMER_A_CLOCKSOURCE_SMCLK,            // SMCLK Clock Source
						psc,       // SMCLK/48 = 1Mhz
						ccr0,
						TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
						TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // eable CCR0 中断
						TIMER_A_DO_CLEAR                     // Clear Counter
		};

		/* Timer_A Compare Configuration Parameter */
		const Timer_A_CompareModeConfig compareConfig =
		{
						TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
						TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
						TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
						ccr0                                       // 16000 Period
		};
		
//    // 1.增计数模式初始化
//    Timer_A_UpModeConfig upConfig;
//    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //时钟源
//    upConfig.clockSourceDivider = psc;                                                     //时钟分频 范围1-64
//    upConfig.timerPeriod = ccr0;                                                           //自动重装载值（ARR）
//    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //禁用 tim溢出中断
//    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //启用 ccr0更新中断
//    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value

    // 2.初始化定时器A
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upModeConfig);
		/* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A1_BASE, &compareConfig);
		
    // 3.选择模式开始计数
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // 5.开启串口端口中断
    MAP_Interrupt_enableInterrupt(INT_TA1_0);
}

// 6.编写TIMA ISR
void TA1_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    /*开始填充用户代码*/
//    MAP_DMA_enableChannel(0);

        /*开始填充用户代码*/
//    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
//    if(g_flag_DAC8532Status == 1)
//        DAC8532timer_callback();

//    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN0);
//    if(g_flag_DAC8532Status == 1)
//        printf("a");


    /*结束填充用户代码*/
}


//void DAC8532timer_callback(void)
//{
//   static i = 0;
//   if(i<DACN)
//   {
//       MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
//       MAP_DMA_setChannelTransfer(DMA_CH2_TIMERA1CCR0 | UDMA_PRI_SELECT,UDMA_MODE_BASIC,
//                                  g_u8dac8532txData[i],
//                                  (void *) MAP_SPI_getTransmitBufferAddressForDMA(EUSCI_B0_BASE),
//                                  MAP_SPI_MSG_LENGTH);
//   }
//   else
//   {
//       Timer_A_stopTimer(TIMER_A1_BASE);
//       Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
//       g_flag_DAC8532Status = 2;
//       printf("dacend!");
//   }
//   i++;
//}









/*********************************************************************************************************/

/**************************************         TIMA0          *******************************************/

void TimA0_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*初始化引脚*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); //通道1
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //通道2
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION); //通道3
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); //通道4

    const Timer_A_PWMConfig TimA0_PWMConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,         //时钟源
        1,                                 //时钟分频 范围1-64
        0,                                 //自动重装载值（ARR）
        TIMER_A_CAPTURECOMPARE_REGISTER_1, //通道1 （注意引脚定义）
        TIMER_A_OUTPUTMODE_TOGGLE_SET,     //输出模式
        50                                  //这里是改变占空比的地方 默认100%
    };
    Timer_A_PWMConfig *User_TimA0_PWMConfig = (Timer_A_PWMConfig*)&TimA0_PWMConfig;
    /*定时器PWM初始化*/
//    User_TimA1_PWMConfig->clockSourceDivider = psc;
//    User_TimA1_PWMConfig->timerPeriod = ccr0;
//    User_TimA1_PWMConfig->dutyCycle = ccr0;
    User_TimA0_PWMConfig->clockSourceDivider = psc;
    User_TimA0_PWMConfig->timerPeriod = ccr0;
    User_TimA0_PWMConfig->dutyCycle = ccr0;

    //第1路 PWM
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &TimA0_PWMConfig); /* 初始化比较寄存器以产生 PWM1 */

    //第2路 PWM
    User_TimA0_PWMConfig->compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2; //通道2 （注意引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &TimA0_PWMConfig);                  /* 初始化比较寄存器以产生 PWM2 */

    //第3路 PWM
    User_TimA0_PWMConfig->compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3; //通道3 （注意引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &TimA0_PWMConfig);                  /* 初始化比较寄存器以产生 PWM3 */

    //第4路 PWM
    User_TimA0_PWMConfig->compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4; //通道4 （注意引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &TimA0_PWMConfig);                  /* 初始化比较寄存器以产生 PWM4 */
}




// 6.编写TIMA ISR
void TA0_0_IRQHandler(void)
{
//    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

//    MAP_DMA_clearInterruptFlag(DMA_CH0_EUSCIB0TX0 & 0x0F);
    //MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /*结束填充用户代码*/
}
/*********************************************************************************************************/
