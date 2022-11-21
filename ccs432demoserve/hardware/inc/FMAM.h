/*
 * FMAM.h
 *
 *  Created on: 2022年7月31日
 *      Author: 14913
 */

#ifndef HARDWARE_INC_FMAM_H_
#define HARDWARE_INC_FMAM_H_


#include "main.h"


#define FINDPEAKTYPE float
uint32_t find_peakband(FINDPEAKTYPE *outdata,uint32_t maxnum, uint32_t *band1, uint32_t *band2,int space,FINDPEAKTYPE standard);



#define ENVELOPETYPE uint16_t


ENVELOPETYPE env_realtime(ENVELOPETYPE x, ENVELOPETYPE rct);
void env_batch(ENVELOPETYPE *x, ENVELOPETYPE *y, uint32_t N, double rct);
void findmaximum(uint16_t *batch,uint16_t *outdata, uint16_t num);
void findmaxandmin(uint16_t *batch,uint16_t num,uint16_t *maxdata,uint16_t *maxindex,uint16_t *mindata,uint16_t *minindex);

double compute_ma();

#endif /* HARDWARE_INC_FMAM_H_ */


