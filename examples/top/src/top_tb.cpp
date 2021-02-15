#include "top.h"

int main() {
  double A[10000], B[10000];
  for (int i = 0; i < 10000; i++) {
    A[i] = (i < 900) ? i + 1 : 0;
    B[i] = A[i];
  }

loop_0:
  for (int i = 0; i < 1000; i++) {
    double e = B[i];
    if (i > 5)
      B[5 * i + 5] =
          (((((e + 0.64) * e + 0.7) * e + 0.21) * e + 0.33) * e + 0.25) * e +
          0.125;
    else
      B[5 * i + 5] = e * e;
  }

  top(A);

  int res = 0;
  for (int i = 0; i < 10000; i++) {
    res += (A[i] == B[i]);
  }

  if (res == 10000)
    return 0;
  else
    return res + 1;
}
