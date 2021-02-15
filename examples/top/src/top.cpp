#include "top.h"
void top(double vec[10000]) {
loop_1:
  for (int i = 0; i < 1000; i++) {
#pragma HLS PIPELINE II=3
#pragma HLS DEPENDENCE variable = vec array inter false

    double e = vec[i];
    if (i > 5)
      vec[5 * i + 5] =
          (((((e + 0.64) * e + 0.7) * e + 0.21) * e + 0.33) * e + 0.25) * e +
          0.125;
    else
	    vec[5 * i + 5] = e * e;
  }
}
