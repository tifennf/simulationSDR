#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>

#include <CLI/CLI.hpp>

#include "simulationSDR/transmitter.h"
#include "simulationSDR/receiver.h"

using namespace simulationSDR;

int main(int argc, char ** argv){
    CLI::App app("Simulateur Monte Carlo");
    
    /* Declaration des options
       * Declaration de la variable avec sa valeur par défaut
       * Ajout de l'option à l'application
       * Verification optionnelle
    */

    float min_SNR = 0;
    app.add_option("-m", min_SNR, "Min SNR");
    float max_SNR = 0;
    app.add_option("-M", max_SNR, "Max SNR");

    float step_val = 1;
    app.add_option("-s", step_val, "Step between two SNR points");

    uint32_t f_max = 10;
    app.add_option("-e", f_max, "Number of frame errors to reach to explore one SNR point");

    size_t K = 4;
    app.add_option("-K", K, "Number of information bits");
    size_t N = 12;
    app.add_option("-N", N, "Codeword size");

    std::string rep = "rep-hard";
    app.add_option("-D", rep, "Decoder type")
      ->check(CLI::IsMember({"rep-hard", "rep-soft"})); // Verifie que la valuer est l'une des deux options

    CLI11_PARSE(app, argc, argv);

    if (N % K) {
      std::cerr << "N must be a multiple of K" << std::endl;
      return 1;
    }

    /* Simulation */

    // Declaration dynamique des tableaux car taille non constante
    uint8_t* u_k = new uint8_t[K];
    uint8_t* c_n = new uint8_t[N];
    int32_t* x_n = new int32_t[N];
    float* y_n = new float[N];
    float* l_n = new float[N];
    uint8_t* v_k = new uint8_t[K];

    size_t n_reps = N / K;
    float R = K / N;

    float esn, sigma;
    uint64_t n_bit_errors, n_frame_errors;
    uint64_t sim_frame;

    float bit_error_rate, frame_error_rate;

    for( float snr = min_SNR; snr <= max_SNR; snr += step_val) {

      n_bit_errors = 0;
      n_frame_errors = 0;
      sim_frame = 0;

      esn = snr + 10 * log10(R); // bs = 1 here (BPSK)
      sigma = sqrt(1 / (2 * pow(10, esn/10)));
      
      do {

        source_generate(u_k, K);

        codec_repetition_encode(u_k, c_n, K, n_reps);

        modem_BPSK_modulate(c_n, x_n, N);

        channel_AWGN_add_noise(x_n, y_n, N, sigma);

        modem_BPSK_demodulate(y_n, l_n, N, sigma);

        if (rep == "rep-hard") {
          codec_repetition_hard_decode(l_n, v_k, K, n_reps);
        } else {
          codec_repetition_soft_decode(l_n, v_k, K, n_reps);
        }

        monitor_check_errors(u_k, v_k, K, &n_bit_errors, &n_frame_errors);

        sim_frame++;

      } while (n_frame_errors < f_max);

      bit_error_rate = n_bit_errors / (sim_frame * K);
      frame_error_rate = n_frame_errors / sim_frame;

      std::cout << "SNR:" << snr 
        << " /  bit error rate: " << bit_error_rate 
        << " /  frame error rate: " << frame_error_rate << std::endl;
    }

    delete[] u_k;
    delete[] c_n;
    delete[] x_n;
    delete[] y_n;
    delete[] l_n;
    delete[] v_k;

    return 0;
}
