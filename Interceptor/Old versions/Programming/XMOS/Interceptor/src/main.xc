// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#include "stdio.h"
#include "fft.h"

int sq(int x) {
    return x*x;
}

void fftRealTest() {
	int re5[1024];
	int im5[1024];
	for (int i = 0; i < 1023; i ++){
		//GET I2S Data here
		im5[i] = 0;
	}
    timer t;
    int t0,t1;

    printf("FORWARDING\n");
    fftTwiddle(re5, im5, 1024);
    fftForward(re5, im5, 1024, sine_1024);
    t :> t0;
    t :> t1;
    printf("DONE");
}

int main(void) {
    fftRealTest();
    return 0;
}
