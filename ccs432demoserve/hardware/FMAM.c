/*
 * FMAM.c
 *
 *  Created on: 2022年7月31日
 *      Author: 14913
 */



#include "FMAM.h"


/***************************************************************
  *  @brief     find peak band on spectrum
  *  @param     *outdata
  *  @param     maxnum
  *  @param     *band1
  *  @param     *band2
  *  @param     space
  *  @param     standard  峰带标准 大于此值视为在峰带内
  *
  *  @return    bandwidth
  *  @note      备注
  *  @Sample usage:     函数的使用方法
 **************************************************************/


uint32_t find_peakband(FINDPEAKTYPE *outdata,uint32_t maxnum, uint32_t *band1, uint32_t *band2,int space,FINDPEAKTYPE standard)
{
     //0正序 1逆序
    //
    int32_t i = maxnum;
    uint8_t band1flag,band2flag = 0;

//    if(space < 0)  //逆序寻找
//    {
        for(i = maxnum-1;i>1;i--)
        {
            if((*(outdata+i) >= standard) && band2flag==0 )
            {
                *band2 = i;
                band2flag=1;
            }

            if( (*(outdata+i) < standard) && band2flag==1 )
            {
                band1flag++;
            }
            else
            {
                band1flag=0;
            }

            if(band1flag>=8)
            {
                *band1 = i+8;
                break;
            }
        }
//    }
//    else if(space > 0)  //顺序寻找
//    {
//        for(i = 0;i<maxnum;i+=space)
//        {
//            if((*(outdata+i) >= standard) && band1flag==0 )
//            {
//                *band1 = i;
//                band1flag=1;
//            }
//
//            if( (*(outdata+i) < standard) && band1flag==1 )
//            {
//                band2flag++;
//            }
//            else
//            {
//                band1flag = 0;
//            }
//
//            if(band2flag>=5)
//            {
//                *band2 = i-5;
//                break;
//            }
//        }
//    }

    return *band2 - *band1;


}



#define ENVELOPETYPE uint16_t




ENVELOPETYPE env_realtime(ENVELOPETYPE x, ENVELOPETYPE rct)

{

    static double old_y = 0.0;

    if(rct == 0.0)
    {
        old_y = 0.0;
    }
    else
    {
        if(x > old_y)
        {
            old_y = x;
        }
        else
        {
            old_y *= rct / ( rct + 1 );
        }
    }

    return old_y;

}



void env_batch(ENVELOPETYPE *x, ENVELOPETYPE *y, uint32_t N, double rct)

{

    int i;
    y[0] = x[0];

    for(i = 1; i < N; i++)

    {

        if( x[i] > y[i-1])

        {

            y[i] = x[i];

        }

        else

        {

            y[i] = y[i-1] * rct / ( rct + 1 );

        }
//        y[i] = 0.1*y[i-1] + 0.9*y[i];
//        printf("{env:%d}\n",x[i]);
    }

}



double compute_ma()
{
    uint16_t maxdata,maxindex,mindata,minindex=0;
//    findmaximum(&g_u16ADCbuff[0][0],envbatchdata, 1024);
    findmaxandmin(envbatchdata,1024,&maxdata,&maxindex,&mindata,&minindex);
    printf("maxdata %d,maxindex%d ,mindata %d,minindex%d \n",maxdata,maxindex,mindata,minindex);
    double ma = (maxdata-mindata)*1.0/(maxdata+mindata-adc_average*2.0/1023);
    printf("ma %.3f",ma);
    return ma;
}



void findmaxandmin(uint16_t *batch,uint16_t num,uint16_t *maxdata,uint16_t *maxindex,uint16_t *mindata,uint16_t *minindex)
{
    uint16_t i=0;
    *maxdata = 0;
    *mindata = 0xffff;
    for(i=2;i<num;i++)
    {
        if(batch[i] != 0)
        {
            if(*maxdata < batch[i])
            {
                *maxdata = batch[i];
                *maxindex = i;
            }
            if(*mindata > batch[i])
            {
                *mindata = batch[i];
                *minindex = i;
            }
        }
    }
}




void findmaximum(uint16_t *batch,uint16_t *outdata, uint16_t num)
{
    outdata[0] = batch[0];
//    outdata[1] = batch[1];

    uint16_t i =0;
    int beta=0;
    int lastbeta=0;
    for(i=1;i<num;i++)
    {
        beta = batch[i] - batch[i-1];
        if(beta < 0&& lastbeta>0)
        {
            outdata[i] = batch[i-1];
        }
        else
        {
            outdata[i] = 0;
        }
        lastbeta = beta;
    }

}




















