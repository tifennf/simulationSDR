#include "simulationSDR/receiver.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <random>




// add white Gaussian noise
void simulationSDR::channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){

    std::random_device r;

    // Generate a normal distribution around sigma
    std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 e2(seed2);
    std::normal_distribution<> normal_dist(sigma, 1);

    for(int i=0; i<N; i++){
        Y_N[i] = X_N[i] + normal_dist(e2);
    }
}

// demodulator, just copies Y_N in L_N for now
void simulationSDR::modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma){
    for(size_t i=0; i<N;i++){
        L_N[i] = 2*Y_N[i]/(sigma*sigma);
    }
}

// hard decoder: first hard decides each LLR and then makes a majority vote
void simulationSDR::codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps){
    const size_t N = K*n_reps;

    int8_t  hard_L[N];
    for(int i=0; i<N;i++){
        hard_L[i]= L_N[i] >=0 ? 0 : 1;
    }


    // i*K + k = (i,k)
    for(size_t k=0; k<K; k++){
        int8_t e = 0;
        for(size_t i=0; i<n_reps; i++){
            e+= hard_L[i*K+k] == 0 ? 1 : -1;
        }

        V_K[k] = e >= 0 ? 0 : 1;
    }

}

// soft decoder: computes the mean of each LLR to hard decide the bits
void simulationSDR::codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps){
    for(size_t k=0; k<K; k++){
        float moy = 0;
        for(size_t i=0; i<n_reps; i++){
            moy+= L_N[i*K+k];
        }

        V_K[k] = moy >= 0 ? 0 : 1;
    }
}

// update `n_bit_errors` and `n_frame_errors` variables depending on `U_K` and `V_K`
void simulationSDR::monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors){
    uint64_t fe = 0;
    uint64_t be = 0;

    for(size_t k = 0; k < K; k++){
        if(U_K[k] != V_K[k]){
            be++;
            fe = 1;
        }
    }

    *n_frame_errors = fe;
    *n_bit_errors = be;
}
