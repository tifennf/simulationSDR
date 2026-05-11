#include "simulationSDR/transmitter.h"
#include <cstddef>
#include <cstdint>
#include <random>
#include "arm_neon.h"

void simulationSDR::source_generate(uint8_t* U_K, size_t K) {
	std::random_device r;

	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_distr(0, 1);

	for (size_t i = 0; i < K; i++) {
		U_K[i] = uniform_distr(e1);
	}
}

void simulationSDR::source_generate_all_zeros(uint8_t *U_K, size_t K) {
  for (size_t i = 0; i < K; i++) {
    U_K[i] = 0;
  }
}

void simulationSDR::codec_repetition_encode(const uint8_t* U_K, uint8_t* C_N, size_t K,	size_t n_reps) {
	for (size_t k = 0; k < n_reps * K; k++) {
		C_N[k] = U_K[k % K];
	}
}

void simulationSDR::modem_BPSK_modulate(const uint8_t* C_N, int32_t* X_N, size_t N) {
	for (size_t k = 0; k < N; k++) {
		X_N[k] = C_N[k] == 0 ? 1 : -1;
	}
}

void simulationSDR::modem_BPSK_modulate_neon(const uint8_t* C_N, int32_t* X_N, size_t N) {
    for(int n = 0; n<N; n+=16){
        // [1,0,1,0] + [0,-1,0,-1] = [1,-1,1,-1]
        uint8x16_t v = vld1q_u8(C_N);
        uint8x16_t u = vceqzq_u8(v);

        int8x16_t a = (int8x16_t)v;
        int8x16_t b = (int8x16_t)u; // FF -> -1

        int8x16_t res = vqaddq_s8(a,b);
        int8x8_t res_low = vget_low_s8(res);
        int8x8_t res_high = vget_high_s8(res);

        // on étend le résultat sur 16 bits, on obtient deux vecteurs de 128 bits
        int16x8_t x16 = vmovl_s8(res_low);
        int16x8_t y16 = vmovl_s8(res_high);

        int16x4_t x16_low = vget_low_s16(x16);
        int16x4_t y16_high = vget_high_s16(y16);

        // on étend le résultat sur 32 bits, on obtient deux vecteurs de 128 bits
        int32x4_t x32 = vmovl_s16(x16_low);
        int32x4_t y32 = vmovl_s16(y16_high);
        vst1q_s32(X_N+n*16    , x32);
        vst1q_s32(X_N+n*16 + 8, y32);
    }


}

void simulationSDR::modem_BPSK_modulate_all_ones(const uint8_t *C_N, int32_t *X_N, size_t N) {
  for (size_t n = 0; n < N; n++) {
    X_N[n] = 1;
  }
}
