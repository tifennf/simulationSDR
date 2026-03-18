#include "simulationSDR/cencoder.h"
#include <cstddef>
#include <random>

void simulationSDR::source_generate(uint8_t *U_K, size_t K) {
    std::random_device r;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_distr(0,1);

    for(size_t i=0; i<K;i++){
        U_K[i] = uniform_distr(e1);
    }
}

void simulationSDR::codec_repetition_encode(const uint8_t *U_K, uint8_t *C_N, size_t K, size_t n_reps) {
    for(size_t k=0; k<n_reps;k+=K){
        for(size_t i=0; i<K; i++){
            C_N[k+i]= U_K[i];
        }
    }
}

void simulationSDR::modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N){
    for(size_t k=0; k<N; k++){
        X_N[k] = C_N[k] == 0 ? 1 : -1;
    }
}
