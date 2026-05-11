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

void source_generate_all_zeros(uint8_t *U_K, size_t K) {
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
        int8x16_t v = vld1q_s8(C_N);
        v = (int8x16_t)vceqz_s8(v);
        int8x16_t u = vmovn(v);
        u = vmul_n(u, -1);

        int8x16_t res_8 = vqaddq_s8(v,u);

        // on étend le résultat sur 16 bits puis sur 32 bits
        int16x8_t res_16 = vmovl_s8(res_8);
        int16x4_t res_16_low = vget_low_s16(vec_16);
        int16x4_t res_16_high = vget_high_s16(vec_16);

        int32x4_t res_32_low = vmovl_s16(res_16_low);
        int32x4_t res_32_high = vmovl_s16(res_16_high);

        vst1q_s32(X_N+n*16    , res_32_low);
        vst1q_s32(X_N+n*16 + 8, res_32_high);
    }


}

void simulationSDR::modem_BPSK_modulate_all_ones(const uint8_t *C_N, int32_t *X_N, size_t N) {
  for (size_t n = 0; n < N; n++) {
    X_N[n] = 1;
  }
}
