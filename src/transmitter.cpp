#include "simulationSDR/transmitter.h"
#include <cstddef>
#include <cstdint>
#include <random>
#include "arm_neon.h"
#include <iostream>
#include <cstdio>

void display_r8_K(int8x16_t r8_128, std::string format, size_t K) {
	int8_t* T = new int8_t[K];
	vst1q_s8(T, r8_128);
	for (size_t i = 0; i < K; i++) {
		printf(format.c_str(), T[i]);
	}

	delete[] T;
}

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
    for(size_t n = 0; n<N; n+=16){
        // [1,0,1,0] + [0,-1,0,-1] = [1,-1,1,-1]
        uint8x16_t v = vld1q_u8(C_N + n); // Load 16 elements d'un octet
        uint8x16_t u = vceqzq_u8(v); // Convertit les 0 en 0xFF et les 1 en 0x00

        int8x16_t a = (int8x16_t)v;
        int8x16_t b = (int8x16_t)u; // FF -> -1
	
	int8x16_t a_inv = vnegq_s8(a);	
	int8x16_t b_inv = vnegq_s8(b);	

//	std::cout << "s = ";
//	display_r8_K(a, "%d\t", N);
//	std::cout << std::endl;

	int8x16_t res = vqaddq_s8(a_inv,b_inv);
//	std::cout << "r = ";
//	display_r8_K(res, "%d\t", N);
//	std::cout << std::endl;
	// 1. Première expansion : 8-bit vers 16-bit
	int8x8_t res_low = vget_low_s8(res);  // Récupère les éléments 0-7
	int8x8_t res_high = vget_high_s8(res); // Récupère les éléments 8-15

	// On étend sur 16 bits (on obtient deux registres de 128 bits)
	int16x8_t res16_low = vmovl_s8(res_low);
	int16x8_t res16_high = vmovl_s8(res_high);

	// 2. Deuxième expansion : 16-bit vers 32-bit
	// Il faut séparer CHAQUE vecteur 16 bits en deux (low et high)
	int16x4_t res16_ll = vget_low_s16(res16_low);   // éléments 0-3
	int16x4_t res16_lh = vget_high_s16(res16_low);  // éléments 4-7
	int16x4_t res16_hl = vget_low_s16(res16_high);  // éléments 8-11
	int16x4_t res16_hh = vget_high_s16(res16_high); // éléments 12-15

	// On étend sur 32 bits (on obtient QUATRE registres de 128 bits)
	int32x4_t out0 = vmovl_s16(res16_ll); // Contient les éléments 0-3
	int32x4_t out1 = vmovl_s16(res16_lh); // Contient les éléments 4-7
	int32x4_t out2 = vmovl_s16(res16_hl); // Contient les éléments 8-11
	int32x4_t out3 = vmovl_s16(res16_hh); // Contient les éléments 12-15

	// 3. Sauvegarde en mémoire
	// n avance de 16, donc X_N + n pointe exactement au bon endroit pour ce bloc
	vst1q_s32(X_N + n,      out0);
	vst1q_s32(X_N + n + 4,  out1);
	vst1q_s32(X_N + n + 8,  out2);
	vst1q_s32(X_N + n + 12, out3);
    }


}

void simulationSDR::modem_BPSK_modulate_all_ones(const uint8_t *C_N, int32_t *X_N, size_t N) {
  for (size_t n = 0; n < N; n++) {
    X_N[n] = 1;
  }
}
