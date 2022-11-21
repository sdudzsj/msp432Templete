/*
 * spi.c
 *
 *  Created on: 2022年7月5日
 *      Author: 14913
 */



/* DriverLib Includes */
#include <dac8532dma.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <string.h>
#include <stdbool.h>





#define SYNC_PORT GPIO_PORT_P5
#define SYNC_PIN GPIO_PIN0




/* SPI Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{ EUSCI_B_SPI_CLOCKSOURCE_SMCLK, 48000000, 2000000,
        EUSCI_B_SPI_MSB_FIRST,
        EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, EUSCI_B_SPI_3PIN };

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];

uint8_t g_flag_DAC8532Status = 0;

uint32_t isrCounter = 0;

uint8_t g_u8dac8532txData[DACN][MAP_SPI_MSG_LENGTH] = {1,2,3};
uint8_t msrxData[26] = {0};

void my_spi0_master_config(void);

void dac8532_start()
{
    //设置片选
    MAP_GPIO_setAsOutputPin(SYNC_PORT,SYNC_PIN);

    TimA1_Int_Init(10, 48);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
    my_spi0_master_config();

    g_flag_DAC8532Status = 1;
}


void my_spi0_master_config(void)
{
#if 1
    volatile uint32_t ii;

    /* Configure CLK, MOSI & MISO for SPI0 (EUSCI_B0) */

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
            GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);


    /* Configuring SPI module */
    MAP_SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);

    /* Enable the SPI module */
    MAP_SPI_enableModule(EUSCI_B0_BASE);

    /* Configuring DMA module */
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

    /* Assign DMA channel 0 to EUSCI_B0_TX0, channel 1 to EUSCI_B0_RX0 */
    MAP_DMA_assignChannel(DMA_CH0_EUSCIB0TX0); //映射外设通道
    MAP_DMA_assignChannel(DMA_CH1_EUSCIB0RX0);


    /* Setup the TX transfer characteristics & buffers */
    //设置定时器A1触发DMA
    MAP_DMA_setChannelControl(DMA_CH0_EUSCIB0TX0 | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(DMA_CH0_EUSCIB0TX0 | UDMA_PRI_SELECT,UDMA_MODE_BASIC,
                               g_u8dac8532txData[0],
                               (void *) MAP_SPI_getTransmitBufferAddressForDMA(EUSCI_B0_BASE),
                               MAP_SPI_MSG_LENGTH);

    /* Setup the RX transfer characteristics & buffers */
    MAP_DMA_setChannelControl(DMA_CH1_EUSCIB0RX0 | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(DMA_CH1_EUSCIB0RX0 | UDMA_PRI_SELECT,
                              UDMA_MODE_BASIC,
                              (void *) MAP_SPI_getReceiveBufferAddressForDMA(EUSCI_B0_BASE),
                              msrxData,
                              MAP_SPI_MSG_LENGTH);


    /* Enable DMA interrupt */
    MAP_DMA_assignInterrupt(INT_DMA_INT0, 0);
    MAP_DMA_clearInterruptFlag(DMA_CH0_EUSCIB0TX0 & 0x0F);

    /* Assigning/Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_DMA_INT0);
    MAP_DMA_enableInterrupt(INT_DMA_INT0);

    MAP_DMA_enableChannel(1);
    MAP_DMA_enableChannel(0);

#elif 0
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
   MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
           GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
   MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
               GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

   //设置片选
   MAP_GPIO_setAsOutputPin(SYNC_PORT,SYNC_PIN);

   /* Configuring SPI in 3wire master mode */
   SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);

   /* Enable SPI module */
   SPI_enableModule(EUSCI_B0_BASE);

   /* Enabling interrupts */
//   SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_SPI_RECEIVE_INTERRUPT);
//   Interrupt_enableInterrupt(INT_EUSCIB0);
//   Interrupt_enableSleepOnIsrExit();
   //![Simple SPI Example]
#endif


}

void DMA_INT0_IRQHandler(void)
{
    static uint32_t i = 0;
    MAP_DMA_clearInterruptFlag(0);
    MAP_DMA_clearInterruptFlag(1);

//    MAP_Interrupt_disableInterrupt(INT_DMA_INT0);
//    MAP_DMA_disableInterrupt(INT_DMA_INT0);

    g_flag_DAC8532Status = 1;
    /* Disable the interrupt to allow execution */
//    MAP_GPIO_setOutputHighOnPin(SYNC_PORT,SYNC_PIN);
    i++;
//    printf("dac %d ",i);
    if(i>=2)
    {
        i=0;
        MAP_GPIO_setOutputHighOnPin(SYNC_PORT,SYNC_PIN);
    }

//    if(i >=1)
//    {
////        Timer_A_stopTimer(TIMER_A1_BASE);
////        Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
////        MAP_Interrupt_disableInterrupt(INT_DMA_INT1);
////        MAP_DMA_disableInterrupt(INT_DMA_INT1);
//        g_flag_DAC8532Status = 2;
//        printf("dacend!");
//    }
//    else
//    {
//
//        MAP_DMA_setChannelTransfer(DMA_CH2_TIMERA1CCR0 | UDMA_PRI_SELECT,UDMA_MODE_BASIC,
//                           g_u8dac8532txData[i],
//                           (void *) MAP_SPI_getTransmitBufferAddressForDMA(EUSCI_B0_BASE),
//                           MAP_SPI_MSG_LENGTH);
//        MAP_GPIO_setOutputLowOnPin(SYNC_PORT,SYNC_PIN);
//    }
}
















