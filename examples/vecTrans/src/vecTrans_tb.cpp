#include "vecTrans.h"

int main(){
   double A[1000], B[1000];
   for (int i = 0; i < 1000; i++){
   		A[i] = i - 500;
   		B[i] = i - 500;
   }

   for (int i = 0; i < 100; i++){
	    double a = A[i];
	    if (a > 100){
	        double b = (((((a+0.64)*a+0.7)*a+0.21)*a+0.33)*a+0.25)*a+0.125;
	        A[i+63] = b;
	    }
	    else{
	        double b = a*a;
	        A[i+9] = b;
	    }
	}

	vecTrans(B);

	int res = 0;
	for (int i = 0; i < 1000; i++)
   		res += (A[i] == B[i]);
   

   if (res == 1000)
   		return 0;
   else
   		return res;
}
