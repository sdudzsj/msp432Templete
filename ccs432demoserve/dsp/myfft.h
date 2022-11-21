

#ifndef _MYFFT_H

#define _MYFFT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <ti/devices/msp432p4xx/inc/msp432.h>
/* Include the iqmathlib header files */
#include <ti/iqmathlib/QmathLib.h>
#include <ti/iqmathlib/IQmathLib.h>

/* Access the real and imaginary parts of an index into a complex array. */
#define RE(x)           (((x)<<1)+0)    // access real part of index
#define IM(x)           (((x)<<1)+1)    // access imaginary part of index
/* Specify the sample size and sample frequency. */
#define SAMPLES         256              // power of 2 no larger than 256
#define SAMPLE_FREQ     8192            // no larger than 16384

extern _q qInput[SAMPLES*2];                   // Input buffer of complex values
extern _q qMag[SAMPLES/2];                     // Magnitude of each frequency result
extern _q qPhase[SAMPLES/2];                   // Phase of each frequency result

extern int16_t fftdata[SAMPLES];

extern void cFFT(_q *input, int16_t n);



#endif /*_MYFFT_H*/








