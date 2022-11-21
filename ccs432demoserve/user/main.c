/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#include <ti/iqmathlib/QmathLib.h>
#include <ti/iqmathlib/IQmathLib.h>

#include "myfft.h"

#include "stdio.h"

void fft_Display(void);
void dac_data_process(void);
double mafilter(double nowma);
double mffilter(double nowmf);

uint8_t testmode = 1;

uint8_t g_flag_Serve = 0;
float duty = 0;
uint32_t ccr0 = 2000;

float myfftoyutdata[256];
uint16_t envbatchdata[1024];
uint32_t adc_average=0;

int main(void)
{

    /* Stop Watchdog  */
    //MAP_WDT_A_holdTimer();
    SysInit();
    delay_init();
    uart_init(115200);
    printf("hello word!\n");

    KEY_Init(KEY_MODE_INT);
    //初始化定时器
    TimA1_Int_Init(96, 1);
    //初始化定时器 timA触发

    adc_dma_init(256);
    ADC_Config();


//    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 5);

    MAP_Interrupt_enableMaster(); // 开启总中断

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
//    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
//    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    OLED_Init();
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    OLED_Refresh();
    OLED_DrawLine(20,0,20,80);
    OLED_Refresh();


    OLED_ShowString(0,0,"START NOW",16);
    OLED_Refresh();


    g_flag_Serve = 1;

    uint16_t fftband1,fftband2,fftbandwidth =0;
    uint16_t frquence=0;


    while (1)
    {
    /*开始填充用户代码*/




      if(testmode == 0)
      {
          OLED_ShowString(0,0,"Please press",16);
          OLED_ShowString(0,20," key to start",16);
          OLED_Refresh();
      }
      else if(testmode == 1)
      {
          if(g_flag_ADCStatus == 0)
              adc_restart();
          if(g_flag_ADCStatus == 2)
          {
              g_flag_ADCStatus = 0;

              findmaximum(&g_u16ADCbuff[0][0],envbatchdata, 1024);
              uint32_t i=0;
              adc_average=0;
              for(i=1; i <ADCN; i++)
              {
                  adc_average+=g_u16ADCbuff[0][i];
              }

              double ma,nowma = 0;

              nowma = 1.1*compute_ma();

              ma = mafilter(nowma);

              OLED_ShowString(0,0,"AM mode",16);
              OLED_ShowString(0,20,"ma",16);

              char string[10];
              sprintf(string,"%2.3f",ma);
//              OLED_ShowString(0,40,"0.3",16);
              OLED_ShowString(20,20,string,16);
//              OLED_ShowFloat(20,20,ma,16);
              OLED_Refresh();
              delay_ms(500);
          }
          MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);

      }
      else if(testmode == 2)
      {
          uint32_t f_frequence = 5;
          MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);

          uint16_t bandwidth = 0;
          OLED_ShowString(0,0,"FM mode",16);
          OLED_Refresh();
            if(g_flag_ADCStatus == 0 && g_flag_cap == 0)
            {
//                //初始化定时器
                TimA1_Int_Init(96, 1);
                //初始化定时器 timA触发

                adc_dma_init(256);
                ADC_Config();

            }
            if(g_flag_ADCStatus == 2)
            {
                g_flag_ADCStatus = 0;

                fft_Display();
                uint32_t firstband,secondband,mybandwidth=256;

                mybandwidth = find_peakband(myfftoyutdata,256, &firstband,  &secondband,-1,0.11);
                fftbandwidth = mybandwidth-2;

                printf("fftbandwidth:%d ,%d,%d\n",mybandwidth,firstband,secondband);




                TimA2_Cap_Init();
//                g_flag_cap = 2;

            }

            if(g_flag_cap == 2)
            {
                g_flag_cap=0;
                uint16_t i =0;
                uint32_t sumcap =0;
                uint16_t maxdata=0;
                uint16_t mindata =0xffff;
                for(i =1;i<10;i++)
                {
                    if(maxdata <TIM2capdata[i]-TIM2capdata[i-1])
                    {
                        maxdata = TIM2capdata[i]-TIM2capdata[i-1];
                    }
                    if(mindata > TIM2capdata[i]-TIM2capdata[i-1])
                    {
                        mindata = TIM2capdata[i]-TIM2capdata[i-1];
                    }
                    sumcap += (TIM2capdata[i]-TIM2capdata[i-1]);

                }
                f_frequence = 505/((sumcap-mindata-maxdata)/7.0);
//                frquence = 5;


            }
            double mf = fftbandwidth*1.0/f_frequence;

            OLED_ShowString(0,0,"FM mode",16);
            OLED_ShowString(0,20,"mf",16);

            char string[10];
            sprintf(string,"%2.3f",mf);
//              OLED_ShowString(0,40,"0.3",16);
            OLED_ShowString(20,20,string,16);

//                OLED_ShowFloat(20,20,mf,16);
            OLED_ShowString(0,40,"maxfre",16);

            char stringfft[10];
          sprintf(stringfft,"%.1f",fftbandwidth*1.0);
//              OLED_ShowString(0,40,"0.3",16);
          OLED_ShowString(40,40,stringfft,16);


//                OLED_ShowFloat(20,40,fftbandwidth,16);
            OLED_ShowString(80,40,"k",16);
            OLED_Refresh();


      }

      else if(testmode == 3)
      {
          uint32_t f_frequence = 3;
          OLED_ShowString(0,0,"AUTO mode",16);
        //初始化定时器
         TimA1_Int_Init(96, 1);
         //初始化定时器 timA触发

         adc_dma_init(256);
         ADC_Config();
          uint8_t mymode = 0;

          double ma = 0;

          if(g_flag_ADCStatus == 0)
                adc_restart();
          if(g_flag_ADCStatus == 2)
          {
             g_flag_ADCStatus = 0;

             findmaximum(&g_u16ADCbuff[0][0],envbatchdata, 1024);
             uint32_t i=0;
             adc_average=0;
             for(i=1; i <ADCN; i++)
             {
                adc_average+=g_u16ADCbuff[0][i];
             }

             ma = compute_ma();
             if(ma > 0.4 && ma <1.1)
             {
                 mymode = 1;
             }
             else
             {
                 fft_Display();
                 uint32_t firstband,secondband,myfftbandwidth=256;
                 myfftbandwidth = find_peakband(myfftoyutdata,256, &firstband,  &secondband,-1,0.11);
                 fftbandwidth = myfftbandwidth-2;
                 printf("fftbandwidth %d\n",fftbandwidth);

                 if(fftbandwidth>5&&fftbandwidth<500)
                 {
                     mymode = 2;
                     MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
                     TimA2_Cap_Init();
                     //g_flag_cap =2;   ///
                 }

                 else
                 {
                     mymode = 3;
                 }
             }

          }

          if(mymode == 2 && g_flag_cap == 2 )
          {
              g_flag_cap=0;
              uint16_t i =0;
              uint32_t sumcap =0;
              uint16_t maxdata=0;
              uint16_t mindata =0xffff;
              for(i =1;i<10;i++)
              {
                  if(maxdata <TIM2capdata[i]-TIM2capdata[i-1])
                  {
                      maxdata = TIM2capdata[i]-TIM2capdata[i-1];
                  }
                  if(mindata > TIM2capdata[i]-TIM2capdata[i-1])
                  {
                      mindata = TIM2capdata[i]-TIM2capdata[i-1];
                  }
                  sumcap += (TIM2capdata[i]-TIM2capdata[i-1]);

              }
              f_frequence = 505/((sumcap-mindata-maxdata)/7.0);
//              frquence = 3;
          }




          if(mymode == 1)
          {
              MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
              OLED_Clear();
              OLED_ShowString(0,20,"AM",16);
              OLED_ShowString(0,40,"ma",16);
              char string[10];
              sprintf(string,"%2.3f",ma-0.1);
  //              OLED_ShowString(0,40,"0.3",16);
              OLED_ShowString(40,20,string,16);
//              OLED_ShowFloat(40,20,ma,16);
              OLED_Refresh();
          }
          if(mymode == 2)
          {

              OLED_Clear();
              double mf = fftbandwidth/f_frequence;
              OLED_ShowString(0,20,"FM",16);
              OLED_ShowString(0,40,"mf",16);
              char string2[10];
              sprintf(string2,"%2.3f",mf-0.1);
              OLED_ShowString(40,20,string2,16);
//              OLED_ShowFloat(40,20,mf,16);
              OLED_ShowString(0,40,"maxfre",16);
              char string1[10];
                sprintf(string1,"%.1f",fftbandwidth*1.0);
                OLED_ShowString(40,40,string1,16);
//              OLED_ShowFloat(20,40,fftbandwidth,16);

              OLED_Refresh();
          }

          if(mymode == 3)
          {
              OLED_Clear();
              OLED_ShowString(20,0,"NO",16);
              OLED_Refresh();
            }

//          OLED_ShowString(0,40,"band",16);
//                  char string[10];
//                  sprintf(string,"%2.3f",fftbandwidth);
//                  OLED_ShowString(20,40,string,16);
//                  OLED_Refresh();

      }






//        if(g_flag_ADCStatus == 2)
//        {
//            printf("ADC transform has finished!\n");
//            g_flag_ADCStatus = 0;
//
//
////            env_batch(&g_u16ADCbuff[0][0],envbatchdata,ADCN,800);
//            findmaximum(&g_u16ADCbuff[0][0],envbatchdata, 1024);
//            uint32_t i=0;
//            adc_average=0;
//            for(i=1; i <ADCN; i++)
//            {
//                adc_average+=g_u16ADCbuff[0][i];
//                printf("{env:%d}\n",g_u16ADCbuff[0][i]);
//            }
//            for(i=0; i <ADCN; i++)
//            {
//                printf("{env:%d}\n",envbatchdata[i]);
//            }
//
//            fft_Display();
//
//            fftbandwidth = find_peakband(myfftoyutdata,256, &fftband1,  &fftband2,-1,0.1);
//
//            printf("fftbandwidth:%d band1:%d band2:%d\n",fftbandwidth,fftband1,fftband2);
//
//            double ma =0;
//
//            ma = compute_ma();
//
//
//
//            TimA2_Cap_Init();
//
//            SPI_SW_init();
//            SPIv_WriteData(0x2f);
////            TPL0401_SW_Configuration();
////            TPL0401_WR_Byte(0x8f);
//
//
//            //开启ADC采集
//            //MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
//        }
//        if(g_flag_cap == 2)
//        {
//            g_flag_cap=0;
//            uint16_t i =0;
//            uint32_t sumcap =0;
//            uint16_t maxdata=0;
//            uint16_t mindata =0xffff;
//            for(i =1;i<10;i++)
//            {
//                if(maxdata <TIM2capdata[i]-TIM2capdata[i-1])
//                {
//                    maxdata = TIM2capdata[i]-TIM2capdata[i-1];
//                }
//                if(mindata > TIM2capdata[i]-TIM2capdata[i-1])
//                {
//                    mindata = TIM2capdata[i]-TIM2capdata[i-1];
//                }
//                sumcap += (TIM2capdata[i]-TIM2capdata[i-1]);
//
//            }
//            printf("timcap: %f \n",505/((sumcap-mindata-maxdata)/7.0) );
//        }




        delay_ms(10);

    /*停止填充用户代码*/
    }
}










void fft_Display()
{
    //FFT
    uint32_t i=0;
    for(i=0; i <SAMPLES; i++)
    {
       fftdata[i] = g_u16ADCbuff[0][i];
       qInput[RE(i)] = fftdata[i];
       qInput[IM(i)] = 0;
       printf("{in:%.5lf}\n",g_u16ADCbuff[0][i]*3.3/16384);
//       printf("{env:%d}\n",g_u16ADCbuff[0][i]);
    }

    cFFT(qInput, SAMPLES);
    //arm_rfft_f32_app(input, Amp, rate, Phase,(uint32_t)(48000000/48/500));
    printf("F:       A;    P\n");
    for(i=0; i <SAMPLES; i++)
    {
        qMag[i] = _Qmag(qInput[RE(i)], qInput[IM(i)]);
        qPhase[i] = _Qatan2(qInput[IM(i)], qInput[RE(i)]);
        myfftoyutdata[i] = qMag[i]*3.3/16384;
        printf("{FFT:%.3lf}%d\n",qMag[i]*3.3/16384,i);
    }
}



double mafilter(double nowma)
{
    const double errorband = 0.1;
    static double lastma = 0.5;
    static double lastlastma = 0;
    double errornow =  nowma - lastma;
    double errorlast =  lastma - lastlastma;



    if(nowma >0 && nowma<1)
    {

        if(lastlastma == 0&& lastma == 0.5)
        {
            lastma = nowma;
            lastlastma = lastma;
            return lastma;
        }


        if(errorlast < errorband && errorlast > -errorband)
        {
            lastma = nowma;
            lastlastma = lastma;
            return lastma;
        }
        else if(errornow < errorband && errornow > -errorband )
        {
            lastma = (nowma+lastma)/2;
            lastlastma = lastma;
            return lastma;
        }
        else
        {
            return lastma;
        }
    }
    else
        return lastma;

}















