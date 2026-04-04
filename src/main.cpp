<<<<<<< HEAD
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>

#include "simulationSDR/transmitter.h"
#include "simulationSDR/receiver.h"

#define N0 1 // Bruit

#define K 4
#define N 3*K

using namespace simulationSDR;
using namespace std;

int main(){
    uint8_t u_k[K];
    source_generate(u_k, K);

    std::cout << "u_k: ";
    for(auto b : u_k)
        std::cout << (int)b << " ";
    std::cout << std::endl;

    uint8_t c_n[N];
    size_t n_reps = N / K;
    codec_repetition_encode(u_k, c_n, K, n_reps);

    std::cout << "c_n: ";
    for(auto b : c_n)
        std::cout << (int)b << " ";
    std::cout << std::endl;

    int32_t x_n[N];
    modem_BPSK_modulate(c_n, x_n, N);

    std::cout << "x_n: ";
    for(auto b : x_n)
        std::cout << b << " ";
    std::cout << std::endl;

    float SNR = n_reps/N0;
    float sigma = sqrt(1 / (2 * pow(10, SNR/10)));
    std::cout << "sigma = " << std::fixed << setprecision(2) << sigma << std::endl;

    float y_n[N];
    channel_AWGN_add_noise(x_n, y_n, N, sigma);

    std::cout << "y_n: ";
    for(float b : y_n)
        std::cout << std::fixed << setprecision(2) << b << " ";
    std::cout << std::endl;

    float l_n[N];
    modem_BPSK_demodulate(y_n, l_n, N, sigma);

    std::cout << "l_n: ";
    for(float b : l_n)
        std::cout << std::fixed << setprecision(2) << b << " ";
    std::cout << std::endl;

    uint8_t v_k_hard[K];
    codec_repetition_hard_decode(l_n, v_k_hard, K, n_reps);

    std::cout << "v_k_hard: ";
    for(auto b : v_k_hard)
        std::cout << (int)b << " ";
    std::cout << std::endl;

    uint8_t v_k_soft[K];
    codec_repetition_soft_decode(l_n, v_k_soft, K, n_reps);

    std::cout << "v_k_soft: ";
    for(auto b : v_k_soft)
        std::cout << (int)b << " ";
    std::cout << std::endl;

    uint64_t n_bit_errors_hard = 0, n_frame_errors_hard = 0;
    monitor_check_errors(u_k, v_k_hard, K, &n_bit_errors_hard, &n_frame_errors_hard);

    std::cout << "n_bit_errors_hard: " << n_bit_errors_hard << " / n_frame_errors_hard: " << n_frame_errors_hard <<std::endl;
    
    uint64_t n_bit_errors_soft = 0, n_frame_errors_soft = 0;
    monitor_check_errors(u_k, v_k_soft, K, &n_bit_errors_soft, &n_frame_errors_soft);

    std::cout << "n_bit_errors_soft: " << n_bit_errors_soft << " / n_frame_errors_soft: " << n_frame_errors_soft <<std::endl;
    
    return 0;
}
