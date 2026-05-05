#include "simulationSDR/receiver.h"
#include "arm_neon.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>

namespace simulationSDR {

void channel_AWGN_add_noise(const int32_t* X_N, float* Y_N, size_t N, float sigma) {
	std::random_device r;
	std::default_random_engine e1(r());
	std::normal_distribution<float> n(0, sigma);

	for (size_t i = 0; i < N; i++) {
		Y_N[i] = X_N[i] + n(e1);
	}
}

void modem_BPSK_demodulate(const float* Y_N, float* L_N, size_t N, float sigma) {
	float facteur = 2.0f / (sigma * sigma);

	for (size_t i = 0; i < N; i++) {
		L_N[i] = facteur*Y_N[i]; // Pour l'instant nous copions juste les memes valeurs au lieu du LLR
	}
}

void codec_repetition_hard_decode(const float* L_N, uint8_t* V_K, size_t K, size_t n_reps) {
	int vote;
	for (size_t i = 0; i < K; i++) {
		vote = 0;
		for (size_t j = 0; j < n_reps; j++) {
			vote += L_N[(j * K) + i] < 0 ? -1 : 1;
		}

		V_K[i] = vote < 0 ? 1 : 0; // Si 0 alors on choisit 0
	}
}



void codec_repetition_soft_decode(const float* L_N, uint8_t* V_K, size_t K, size_t n_reps) {
	float moy;
	for (size_t i = 0; i < K; i++) {
		moy = 0;
		for (size_t j = 0; j < n_reps; j++) {
			moy += L_N[(j * K) + i];
		}

		V_K[i] = moy < 0 ? 1 : 0; // Si 0 alors on choisit 0
	}
}

void monitor_check_errors(const uint8_t* U_K, const uint8_t* V_K, size_t K, uint64_t* n_bit_errors,
						  uint64_t* n_frame_errors) {
	int fram_err = 0;

	for (size_t i = 0; i < K; i++) {
		if (U_K[i] != V_K[i]) {
			(*n_bit_errors)++;
			fram_err = 1;
		}
	}

	if (fram_err) {
		(*n_frame_errors)++;
	}
}

// hard decoder: first hard decides each LLR and then makes a majority vote
void codec_repetition_hard_decode8_neon(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps) {
    for(size_t k =0; k < K; k+=16){
        // vote cpt
        int8x16_t vote_vec = vdupq_n_s8(0);

        // row sum with vote cpt
        for(size_t i = 0; i < n_reps;i++){
            int8x16_t vec = vld1q_s8(L8_N+i*K + k);
            uint8x16_t mask = vcltzq_s8(vec);

            vote_vec = vaddq_s8(vote_vec, (int8x16_t)mask);
        }

        // final vote for this chunk
        int8_t vote[16]; // we actually count the amount of neg values
        vst1q_s8(vote, vote_vec);
        for(size_t j=0; j<16;j++){
            V_K[j+k] = (-2*vote[j] > (int)n_reps) ? 1: 0;
        }
    }

}
void codec_repetition_soft_decode8_neon(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps){
    for(size_t k =0; k < K; k+=16){
        // vote cpt
        int8x16_t sum_vec = vdupq_n_s8(0);

        // row sum with vote cpt
        for(size_t i = 0; i < n_reps;i++){
            int8x16_t data_row = vld1q_s8(L8_N+i*K + k);

            vote_vec = vaddq_s8(sum_vec, data_row);
        }

        // final vote for this chunk
        int8_t vote[16];
        uint8x16_t mask = vcltzq_s8(sum_vec);
        vst1q_s8(vote, mask);
        for(size_t j=0; j<16;j++){
            V_K[j+k] = vote[j];
        }
    }
}

void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
    for(size_t i=0; i<N; i++){
        L8_N[i] = std::min(std::max(std::round(std::pow(2, f)*L_N[i]), std::pow(2,s-1)), -std::pow(2, s-1) -1 );
    }
}




} // namespace simulationSDR
