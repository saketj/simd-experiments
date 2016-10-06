#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>
using namespace std;

#define NUM_ELEMENTS 1048576  // Currently only multiple of two are supported.

void vectorizedColumnCompare(const int stride, const int n, const char * __restrict__ a, const char * __restrict__ b, char * __restrict__ c) {
  char tmp[n];
  for(int i = 0; i < n; i++) { // Vectorized comparison.
    int val1 = a[i * stride]; 
    int val2 = b[i * stride];
    tmp[i] = val1 >= val2 ? 0x1 : 0x0;
  }
  for (int i = 0, j = 0; i < n; i += 8, ++j) { // Vectorized packing.
    char ch = 0x0;
    ch |= tmp[i];
    ch |= tmp[i+1] << 1;
    ch |= tmp[i+2] << 2;
    ch |= tmp[i+3] << 3;
    ch |= tmp[i+4] << 4;
    ch |= tmp[i+5] << 5;
    ch |= tmp[i+6] << 6;
    ch |= tmp[i+7] << 7;
    c[j] = ch;
  }
}

int main() {
  int n = 65536;
  int a[n], b[n], c[n/8];
  for (int i = 0; i < n; ++i) { // Vectorized initialization.
    a[i] = i % 2;
    b[i] = i % 3;
  }
  char *a_ptr = (char *)a;
  char *b_ptr = (char *)b;
  char *c_ptr = (char *)c;

  int stride = 4; 
  
  vectorizedColumnCompare(stride, n, a_ptr, b_ptr, c_ptr);

  // Verify correctness.
  for (int i = 0; i < n; ++i) { 
    bool is_set = (c_ptr[i/8] & (0x1 << (i % 8))) > 0;
    if (a[i] >= b[i]) {
      assert(is_set == true);
    } else {
      assert(is_set == false);
    }
  }
  printf("Correctness verified.\n");  
  return 0;
}
