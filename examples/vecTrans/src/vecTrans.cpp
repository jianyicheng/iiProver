#include "vecTrans.h"

void vecTrans(double A[1000]){
#pragma HLS ARRAY_PARTITION variable=A block factor=2 dim=1
	for (int i = 0; i < 900; i++){
	//#pragma HLS PIPELINE
	#pragma HLS PIPELINE II=2
	#pragma HLS DEPENDENCE variable=A array inter false
	    double a = A[i];
	    if (a > 100.0){
	    	double b = (((((a+0.64)*a+0.7)*a+0.21)*a+0.33)*a+0.25)*a+0.125;
		A[i+63] = b;
	    }
	   	else {
	        double b = a*a;
	        A[i+9] = b;
	    }
	}
}
