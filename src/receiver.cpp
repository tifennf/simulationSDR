#include "simulationSDR/receiver.h"

#include <random>

namespace simulationSDR {

    void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma) {
      std::random_device r;
      std::default_random_engine e1(r());
      std::normal_distribution<float> n(0, sigma);

      for(size_t i = 0; i < N; i++) {
        Y_N[i] = X_N[i] + n(e1);
      }
    }

    void modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma) {
      for(size_t i = 0; i < N; i++) {
        L_N[i] = Y_N[i]; // Pour l'instant nous copions juste les memes valeurs au lieu du LLR
      }
    }

    void codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps) {
      int vote;
      for(size_t i = 0; i < K; i++) {
        vote = 0;
        for(size_t j = 0; j < n_reps; j++) {
          vote += L_N[(j * K) + i] < 0 ? -1 : 1;
        }

        V_K[i] = vote < 0 ? 1 : 0; // Si 0 alors on choisit 0
      }
    }

    void codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps) {
      float moy;
      for(size_t i = 0; i < K; i++) {
        moy = 0;
        for(size_t j = 0; j < n_reps; j++) {
          moy += L_N[(j * K) + i];
        }

        V_K[i] = moy < 0 ? 1 : 0; // Si 0 alors on choisit 0
      }
    }

    void monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors) {
      int fram_err = 0;
    
      for(size_t i = 0; i < K; i++) {
        if(U_K[i] != V_K[i]) {
          (*n_bit_errors)++;
          fram_err = 1;
        }
      }

      if(fram_err) {
        (*n_frame_errors)++;
      }
    }
}
