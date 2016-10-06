#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdio>
#include <chrono>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <chrono>
#include <memory>
#include <unistd.h>
#include <pthread.h>
using namespace std;


template<class T>
struct thread_args;

template<class T>
void* thread_main(void *args);

static size_t num_trials;
static vector<size_t> inputs;
double median(vector<double> &arr);
void output(vector<pair<size_t, double>> results, size_t type_size, size_t num_threads);

template<class T>
void simpleColumnCompare(size_t n, const T *a, const T *b, vector<bool> &out) {
  for(size_t i = 0; i < n; i++) { // This loop is not vectorized!
    out[i] = a[i] > b[i];
  }
}

template<class T>
double benchmark_trial(size_t num_threads, size_t n) {
  T **a = new T*[num_threads];
  T **b = new T*[num_threads];
  vector<vector<bool>> out_bitset(num_threads, vector<bool>(n, false));
  for (size_t i = 0; i < num_threads; ++i) { 
    a[i] = new T[n];
    b[i] = new T[n];
    for (size_t j = 0; j < n; ++j) {
      a[i][j] = (i*j) % 2;
      b[i][j] = (i*j) % 3;
    }
  }  
  
  // Create threads.
  pthread_t threads[num_threads];
  pthread_attr_t attr;
  cpu_set_t cpus;
  pthread_attr_init(&attr);
 
  thread_args<T> **args = new thread_args<T>*[num_threads];
  for (size_t i = 0; i < num_threads; ++i) {
    args[i] = new thread_args<T>(i, n, a[i], b[i], out_bitset[i]);
    CPU_ZERO(&cpus);
    CPU_SET(i, &cpus);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
    pthread_create(&threads[i], &attr, thread_main<T>, args[i]);
  }
  // Wait for all threads to finish.
  for (size_t i = 0; i < num_threads; ++i) {
    pthread_join(threads[i], nullptr);
  }
  
  // After all threads are finished, measure total_time.
  long min_begin_time = args[0]->begin;
  long max_end_time = args[0]->end;
  for (size_t i = 1; i < num_threads; ++i) {
    min_begin_time = min(min_begin_time, args[i]->begin);
    max_end_time = max(max_end_time, args[i]->end);
  }
  double total_time_in_seconds = ((max_end_time - min_begin_time)) / ((double) pow(10,9));
  
  // Verify correctness.
  for (size_t i = 0; i < num_threads; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (a[i][j] > b[i][j]) {
	assert(out_bitset[i][j] == true);
      } else {
	assert(out_bitset[i][j] == false);
      }
    }
  }
  
  // Cleanup
  for (size_t i = 0; i < num_threads; ++i) {
    delete [] a[i];
    delete [] b[i];
  }
  delete[] a;
  delete[] b;
  delete[] args;

  return total_time_in_seconds;
}

template<class T>
double benchmark(size_t num_threads, size_t n) {
  vector<double> trials;
  for (size_t i = 0; i < num_trials; ++i) {
    trials.push_back(benchmark_trial<T>(num_threads, n));
  }
  return median(trials);
}

template<class T>
vector<pair<size_t, double>> benchmark_runner(size_t num_threads) {
  vector<pair<size_t, double>> results;
  for (size_t i = 0; i < inputs.size(); ++i) {
    size_t n = inputs[i] / sizeof(T);
    results.push_back(make_pair(n, benchmark<T>(num_threads, n)));
  }
  return results;
}

int main(int argc, char **argv) {
  size_t num_threads;
  if (argc > 1) {
    num_threads = stoi(argv[1]); 
  } else {
    num_threads = 20; // default
  } 
  if (argc > 2) { 
    num_trials = stoi(argv[2]);
  } else {
    num_trials = 11; // default
  }
  if (argc > 3) {
    for (int i = 3; i < argc; ++i) {
      inputs.push_back(stoi(argv[i]));
    }
  } else {
    inputs.push_back(100000);
    inputs.push_back(1000000);
    inputs.push_back(10000000);
    inputs.push_back(100000000); // by default
  }

  printf("Benchmark results for uint8 data type for %lu threads:\n", num_threads);
  output(benchmark_runner<uint8_t>(num_threads), sizeof(char), num_threads);

  printf("Benchmark results for uint16 data type for %lu threads:\n", num_threads);
  output(benchmark_runner<uint16_t>(num_threads), sizeof(char), num_threads);

  printf("Benchmark results for uint32 data type for %lu threads:\n", num_threads);
  output(benchmark_runner<uint32_t>(num_threads), sizeof(int), num_threads);
  
  printf("Benchmark results for uint64 data type for %lu threads:\n", num_threads);
  output(benchmark_runner<uint64_t>(num_threads), sizeof(double), num_threads);

  return 0;
}

template<class T>
void* thread_main(void *args) {
  thread_args<T> *init = (thread_args<T> *) args;
  init->begin = static_cast<long>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
  simpleColumnCompare<T>(init->n, init->a, init->b, init->out);
  init->end = static_cast<long>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
  return nullptr;
}

void output(vector<pair<size_t, double>> results, size_t type_size, size_t num_threads) {
  for (size_t i = 0; i < results.size(); ++i) {
    double memory_accessed_per_thread = ((results[i].first * type_size * 2.0) + (results[i].first / 8.0)) / (1024.0 * 1024.0 * 1024.0);
    double memory_accessed = memory_accessed_per_thread * num_threads;
    double median_time = results[i].second;
    double bandwidth = memory_accessed / median_time;
    printf("%lu, %f\n", results[i].first, bandwidth);
  }
}

double median(vector<double> &arr) {
  sort(arr.begin(), arr.end());
  size_t n = arr.size();
  if (n % 2 == 0) {
    return (arr[n/2 - 1] + arr[n/2]) / 2.0f;
  } else {
    return arr[n/2];
  }
}

template<class T>
struct thread_args {
  int idx;
  int n;
  T *a;
  T *b;
  vector<bool> &out;
  long begin;
  long end;
  thread_args(int _idx, int _n, T *_a, T *_b, vector<bool> &_out) 
    : idx(_idx), n(_n), a(_a), b(_b), out(_out), begin(0), end(0) {}
};
