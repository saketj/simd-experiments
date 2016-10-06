#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>
#include <chrono>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <bitset>
using namespace std;

#define NUM_ELEMENTS 1048576  // Currently only multiple of two are supported.

template<class T>
void simpleColumnCompare(const int n, const T *a, const T *b, bitset<NUM_ELEMENTS> &out_bitset) {
  for(int i = 0; i < n; i++) { // This loop is not vectorized!
    out_bitset.set(i, a[i] >= b[i]);
  }
}

int main() {
  int n = NUM_ELEMENTS;
  int *a = new int[n];
  int *b = new int[n];
  for (int i = 0; i < n; ++i) { // Vectorized initialization.
    a[i] = i % 2;
    b[i] = i % 3;
  }
  
  bitset<NUM_ELEMENTS> out_bitset;
  
  simpleColumnCompare<int>(n, a, b, out_bitset);
  
  // Verify correctness.
  for (int i = 0; i < n; ++i) { 
    if (a[i] >= b[i]) {
      assert(out_bitset.test(i) == true);
    } else {
      assert(out_bitset.test(i) == false);
    }
  }
  printf("Correctness verified.\n");  
  delete a;
  delete b;
  return 0;
}
