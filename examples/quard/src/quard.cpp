#include "quard.h"
void quard(double vec[10000]){
  loop_0: for (int i = 0; i < 100; i++){
#pragma HLS PIPELINE II=3
#pragma HLS dependence variable=vec inter false
  	double e = vec[i];
	if (e > 10.0)
		vec[i*i+D] = e*e*e;
	else
		vec[i+OFFSET] = (((((e+0.64)*e+0.7)*e+0.21)*e+0.33)*e+0.25)*e+0.125;
  }
}
