/****************************************************/
//MSP432P401R
//ADC采集
// author：luxun
// luxun59@126.com
// 创建日期:2022/5/25
/****************************************************/



/*********************
 *
 * 最大采集电压 3.3V
 *
 * ADC采集引脚：
 * 单路 为 P4.5           A8
 * 双路 为 P4.5 P4.4      A8 A9
 * 三路 为 P4.5 P4.4 P4.3 A8 A9 A10
 *
 ************************/
 
#include "adc.h"
#include "usart.h"
#include "delay.h"
 

uint8_t g_flag_ADCStatus = 0;
uint16_t g_u16ADCbuff[ADCM][ADCN] = {0};

static uint32_t f_index_SampleTimes = 0;

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, ADCN)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 1024
#elif defined(__GNUC__)
__attribute__((aligned(1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];

volatile bool recv_done_flag = 0;
uint16_t g_u16ADCbuff[ADCM][ADCN]; //FFT输入数组




void adc_restart()
{
    DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_BASIC, (void *)&ADC14->MEM[0], (void *)&g_u16ADCbuff[0], ADCN);
    DMA_enableChannel(7);
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE); //开启计数
}











// DMA初始化函数
void adc_dma_init(uint16_t len)
{
  /* Configuring DMA module */
  MAP_DMA_enableModule();
  MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);
  //    DMA_disableChannelAttribute(DMA_CH7_ADC14, UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);
  MAP_DMA_assignChannel(DMA_CH7_ADC14);  //映射外设通道
  MAP_DMA_setChannelControl(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);  //数据大小、源地址、、增量、目标地址增量、仲裁大小和使用突发标志
  MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_BASIC, (void *)&ADC14->MEM[0], (void *)g_u16ADCbuff, ADCN);
  MAP_DMA_assignInterrupt(INT_DMA_INT1, 7);
  MAP_DMA_clearInterruptFlag(DMA_CH7_ADC14 & 0x0F);
  MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
  MAP_DMA_enableInterrupt(INT_DMA_INT1);
  MAP_DMA_enableChannel(7);
}



// 这里时尝试不开乒乓模式 采集更多的点数 但是时间来不及了 于是做一半
void DMA_INT1_IRQHandler(void)
{
  static uint8_t i = 0;
  ADC14_clearInterruptFlag(ADC_INT0);
  DMA_clearInterruptFlag(7);
  printf("adc");
  g_flag_ADCStatus = 1;
  ++i;
  if (i != ADCM)
  {
    DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_BASIC, (void *)&ADC14->MEM[0], (void *)&g_u16ADCbuff[i], 128);
    DMA_enableChannel(7);
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE); //开启计数
    return;
  }
  else
  {
    Timer_A_stopTimer(TIMER_A1_BASE);
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
    ADC14_clearInterruptFlag(ADC_INT0);

    g_flag_ADCStatus = 2;
    i = 0;
  }


  // DMA_disableChannel(7); // dma will auto disable channel if complete

  /* Disable the interrupt to allow execution */
  // Interrupt_disableInterrupt(INT_DMA_INT1);
  // DMA_disableInterrupt(INT_DMA_INT1);
}








// ADC初始化函数
void ADC_Config(void)
{
  /* Initializing ADC (MCLK/1/1) */
  MAP_ADC14_enableModule();                                                                 // 使能ADC14模块
  MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE); // 初始化ADC时钟 超频至48MHz

  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN6, GPIO_TERTIARY_MODULE_FUNCTION); // 模拟输入
  MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);                                                    // 单通道配置 多次转化true
  MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7, false);     // 使用内部2.5V电压参考 非差分输入false
  //ADC_VREFPOS_AVCC_VREFNEG_VSS ADC_VREFPOS_INTBUF_VREFNEG_VSS
  MAP_ADC14_enableInterrupt(ADC_INT0);                                                                    // ADC通道0的中断

//  MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V); // 使用内部2.5V电压参考
//  MAP_REF_A_enableReferenceVoltage();            // 使能内部2.5V电压参考

  MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_TA1_C1, false);  // 使用定时器A通道1作为触发源

  MAP_ADC14_enableConversion(); // 使能开始转换(触发后 自动ADC上电)
  delay_ms(1000);               // 等待稳定
}




//#if ADCM == 1
//static uint32_t f_resultindex[ADCM] = {AI_1_CHANL};
//#elif ADCM == 2
//static uint32_t f_resultindex[ADCM] = {AI_1_CHANL,AI_2_CHANL};
//#elif ADCM == 3
//
//
//
//#endif
//
//
//
//
//static uint16_t resultsBuffer[16];

//void ADC_Config(void)
//{
//	/* 启用浮点运算的FPU */
//    MAP_FPU_enableModule();
//    MAP_FPU_enableLazyStacking();
//
//    /* Initializing ADC (MCLK/1/1) */
//    MAP_ADC14_enableModule();                                                                 //使能ADC14模块
//    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE); //初始化ADC 时钟 分频  通道 2.4MHz
//    // ADC_CLOCKSOURCE_ADCOSC  ADC_CLOCKSOURCE_MCLK
//
//
//#if ADCM == 1
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(AI_1_PORT, AI_1_PIN, GPIO_TERTIARY_MODULE_FUNCTION); //模拟输入
//    MAP_ADC14_configureSingleSampleMode(AI_1_MEM, true);                                                    //单通道配置 多次转化true
//    MAP_ADC14_configureConversionMemory(AI_1_MEM, ADC_VREFPOS_AVCC_VREFNEG_VSS, AI_1_AD, false);       //使用内部电源电压参考 非差分输入false
//    MAP_ADC14_enableInterrupt(AI_1_INT);                                                                    //ADC通道0的中断
//
//
//#elif ADCM == 2
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(AI_1_PORT, AI_1_PIN, GPIO_TERTIARY_MODULE_FUNCTION); //模拟输入
//	  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(AI_2_PORT, AI_2_PIN, GPIO_TERTIARY_MODULE_FUNCTION); //模拟输入
//    MAP_ADC14_configureMultiSequenceMode(AI_1_MEM, AI_2_MEM, true); //多通道配置 多次转化true
//    MAP_ADC14_configureConversionMemory(AI_1_MEM, ADC_VREFPOS_INTBUF_VREFNEG_VSS, AI_1_AD, false);
//    MAP_ADC14_configureConversionMemory(AI_2_MEM, ADC_VREFPOS_INTBUF_VREFNEG_VSS, AI_2_AD, false);
//    MAP_ADC14_enableInterrupt(AI_2_INT);
//
//#elif ADCM == 3
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION); //
//    MAP_ADC14_configureMultiSequenceMode(ADC_MEM8, ADC_MEM10, true);
//    MAP_ADC14_configureConversionMemory(ADC_MEM8, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A8, false);
//    MAP_ADC14_configureConversionMemory(ADC_MEM9, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A9, false);
//		MAP_ADC14_configureConversionMemory(ADC_MEM10, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A10, false);
//    MAP_ADC14_enableInterrupt(ADC_INT10);
//
//#endif
//
////    /* Configuring Sample Timer */
//
//    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
//
//    MAP_ADC14_setPowerMode(ADC_UNRESTRICTED_POWER_MODE);
//
//    /* Enabling Interrupts */
//    MAP_Interrupt_enableInterrupt(INT_ADC14); //ADC模块的中断
//    MAP_Interrupt_enableMaster();
//
//    /* Setting up the sample timer to automatically step through the sequence
//     * convert.
//     */
//    //MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION); //自动触发
//    ADC14_setSampleHoldTrigger(MY_ADC_TRIGGER_SOURCE,false); //定时器触发
//    /* Triggering the start of the sample */
//    MAP_ADC14_enableConversion();        //使能开始转换(触发后 自动ADC上电)
//    //MAP_ADC14_toggleConversionTrigger(); //开启第一次软件触发
//}
//
//void ADC14_IRQHandler(void)
//{
//
//    uint_fast64_t status = MAP_ADC14_getEnabledInterruptStatus();
//    MAP_ADC14_clearInterruptFlag(status);
//
//#if ADCM == 1
//    if (AI_1_INT & status)
//#elif ADCM == 2
//    if (AI_2_INT & status)
//#elif ADCM == 3
//    if (AI_3_INT & status)
//#endif
//    {
//#if ADCM == 1
//        resultsBuffer[0] = ADC14_getResult(AI_1_MEM);
//#else
//        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
//#endif
//			  //开始转换清缓冲区
//			  if(g_flag_ADCStatus == 0)
//				{
//						f_index_SampleTimes = 0;
//				}
//        //设置转换状态
//			  g_flag_ADCStatus = 1;
//
//			  uint8_t i	;
//        for ( i= 0; i < ADCM; i++)
//			  {
//			      g_u16ADCbuff[i][f_index_SampleTimes] = resultsBuffer[f_resultindex[i]];
//            printf("[%d]:%d\r\t", i, resultsBuffer[f_resultindex[i]]);
//
//            if(f_index_SampleTimes >= ADCN)
//						{
//					      MAP_Timer_A_stopTimer ( TIMER_A1_BASE );
//							  g_flag_ADCStatus = 2;
//						}
//			  }
//				//printf("\r\n");
//				//采用次数统计加一
//				f_index_SampleTimes++;
//    }
//}


















