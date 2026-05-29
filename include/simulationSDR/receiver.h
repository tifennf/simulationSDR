#pragma once

#include <cstdint>
#include <cstddef>
#include <thread>

namespace simulationSDR {

    // add white Gaussian noise
    void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma);
    void channel_AWGN_add_noise_neon(const int32_t *X_N, float *Y_N, size_t N, float sigma);


    // demodulator, just copies Y_N in L_N for now
    void modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma);
    void modem_BPSK_demodulate_neon(const float *Y_N, float *L_N, size_t N, float sigma);


    // hard decoder: first hard decides each LLR and then makes a majority vote
    void codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);
    void codec_repetition_hard_decode8(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps);
    void codec_repetition_hard_decode8_neon(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps);

    // soft decoder: computes the mean of each LLR to hard decide the bits
    void codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);
    void codec_repetition_soft_decode8(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps);
    void codec_repetition_soft_decode8_neon(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps);

    // update `n_bit_errors` and `n_frame_errors` variables depending on `U_K` and `V_K`
    void monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors);
    void monitor_check_errors_neon(const uint8_t* U_K, const uint8_t* V_K, size_t K, uint64_t* n_bit_errors,
						  uint64_t* n_frame_errors);
    void monitor_check_errors_neon_atomic(const uint8_t* U_K, const uint8_t* V_K, size_t K, 
                                            std::atomic<uint64_t>* n_bit_errors, std::atomic<uint64_t>* n_frame_errors);
    void monitor_check_errors_atomic(const uint8_t *U_K, const uint8_t *V_K, size_t K, std::atomic<uint64_t> *n_bit_errors, std::atomic<uint64_t> *n_frame_errors);

    // transform numbers from floating-point representation to fixed-point representation
    // `s` is the number of bits used in the quantizer block
    // `f` is the number of bits of the fractional part (`s` >= `f`)
    void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f);


}
