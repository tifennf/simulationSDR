#include<cstdlib>
#include<cstddef>

#include"cencoder.h"

using namespace std;
using namespace simulationSDR;

int main(){

  uint8_t U_K = 0;
  uint8_t C_N = 1;
  
  int32_t X_N = 2;

  size_t K = 3;
  size_t n_reps = 3;
  size_t N = 3;
  
  source_generate(&U_K, K);
  codec_repetition_encode(&U_K, &C_N, K, n_reps);
  modem_BPSK_modulate(&C_N, &X_N, N);

  return 0;
}
