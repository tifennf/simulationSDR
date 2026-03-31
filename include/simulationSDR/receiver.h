#pragma once

#include <cstdint>
#include <cstddef>

namespace simulationSDR {

    // add white Gaussian noise
    void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma);

    // demodulator, just copies Y_N in L_N for now
    void modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma);

    // hard decoder: first hard decides each LLR and then makes a majority vote
    void codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);

    // soft decoder: computes the mean of each LLR to hard decide the bits
    void codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);

    // update `n_bit_errors` and `n_frame_errors` variables depending on `U_K` and `V_K`
    void monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors);


}
