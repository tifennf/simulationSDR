#include"transmitter.h"

#include <iostream>

using namespace std;
using namespace simulationSDR;

#define K 4 
#define N 12

int main(){

  uint8_t u_k[K];
  uint8_t c_n[N];

  size_t n_reps = N/K;

  int32_t x_n[N];
  
  source_generate(u_k, K);
  codec_repetition_encode(u_k, c_n, K, n_reps);
  modem_BPSK_modulate(c_n, x_n, N);

  cout << "u_k: ";
  for(auto i : u_k)
    cout << static_cast<int>(i) << " ";
  cout << endl;

  cout << "c_n: ";
  for(auto i : c_n)
    cout << static_cast<int>(i) << " ";
  cout << endl;

  cout << "x_n: ";
  for(auto i : x_n)
    cout << i << " ";
  cout << endl;

  return 0;
}
