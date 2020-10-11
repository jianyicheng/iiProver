#include "dist_itr.h"

void dist_itr(double A[2000]){
  for (int i=0; i<980; i++){
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable=A array inter false
    double beta = A[i], result;
    if (beta >= 1){
	  result = 1.0;
	}
	else{
	  double is_neg = (beta < 0)?1.0:-1.0;
	  result = ((beta*beta+19.52381)*beta*beta+3.704762)*beta*is_neg;
	}
	A[2*i+5] = result;
  }
}
