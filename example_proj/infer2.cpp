#include <iostream>

#define N 50

using namespace std;

void example(int a[N], int b[N]) {
#pragma HLS INTERFACE m_axi port = a depth = 50
#pragma HLS INTERFACE m_axi port = b depth = 50
  int buff[N];
  for (size_t i = 0; i < N; ++i) {
#pragma HLS PIPELINE II = 1
    buff[i] = a[i];
    buff[i] = buff[i] + 100;
    b[i] = buff[i];
  }
}

int main() {
  int in[N], res[N];
  for (size_t i = 0; i < N; ++i) {
    in[i] = i;
  }

  example(in, res);

  size_t err = 0;
  for (int i = 0; i < N; ++i)
    if (res[i] != i + 100)
      ++err;

  if (err) {
    cout << "Test failed.\n";
    return 1;
  }

  cout << "Test passed.\n";
  return 0;
}
