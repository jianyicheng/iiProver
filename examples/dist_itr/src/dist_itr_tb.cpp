#include "dist_itr.h"
#include <iostream>

int main(){
  double A[2000], B[2000];

  for (int i = 0; i < 2000; i++){
    A[i] = i/2000.0; //std::rand();
    B[i] = A[i];
  }

  dist_itr(A);
  
  for (int i = 0; i < 980; i++){
    double beta = B[i], result;
    if (beta >= 1){
      result = 1.0;
    }
    else{
      double is_neg = (beta < 0)?1.0:-1.0;
      result = ((beta*beta+19.52381)*beta*beta+3.704762)*beta*is_neg;
    }
    B[2*i+5] = result;
  }

  int result = 0;
  for (int i = 0; i < 2000; i++)
    result += (A[i] == B[i]);
  
  if (result == 2000)
    return 0;
  else
    return 1;

}
