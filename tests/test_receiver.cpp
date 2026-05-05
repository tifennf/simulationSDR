#include "gtest/gtest.h"
#include <cstdint>
#include <gtest/gtest.h>

#include "simulationSDR/receiver.h"

#define K 4
#define N 3 * K

namespace simulationSDR {

TEST(ReceiverTest, HardDecode) {
	const float L[N] = {0.2, -1.3, 0.3, 1.2, 0.8, -0.5, -1.5, 0.4, -0.1, 0.2, -1.8, 2};
	uint8_t V[K];

	uint8_t V_ref[K] = {0, 1, 1, 0};

	simulationSDR::codec_repetition_hard_decode(L, V, K, 3);

	for (size_t k = 0; k < K; k++) {
		if (V[k] != V_ref[k]) {
			FAIL() << "HardDecode error at index " << k << ", (" << static_cast<int>(V[k]) << ", "
				   << static_cast<int>(V_ref[k]) << ")";
		}
	}
}

TEST(ReceiverTest, SoftDecode) {
	const float L[N] = {0.2, -1.3, 0.3, 1.2, 0.8, -0.5, -1.5, 0.4, -0.1, 0.2, -1.8, 2};
	uint8_t V[K];

	uint8_t V_ref[K] = {0, 1, 1, 0};

	simulationSDR::codec_repetition_soft_decode(L, V, K, 3);

	for (size_t k = 0; k < K; k++) {
		if (V[k] != V_ref[k]) {
			FAIL() << "SoftDecode error at index " << k << ", (" << static_cast<int>(V[k]) << ", "
				   << static_cast<int>(V_ref[k]) << ")";
		}
	}

	// 1. Définition des dimensions
	const int LIGNES = 3;
	const int COL_ORIGINE = 4;
	const int COL_CIBLE = 16;

	// 3. La matrice finale (3 lignes x 16 colonnes = 48 éléments)
	// Attention: On ne peut pas mettre 'const' ici car on va la remplir dynamiquement
	float large_L[LIGNES * COL_CIBLE];

	// 4. La boucle de duplication
	for (int i = 0; i < LIGNES; ++i) {
		for (int j = 0; j < COL_CIBLE; ++j) {

			// Calcul de l'index dans la grande matrice
			int index_dest = (i * COL_CIBLE) + j;

			// Calcul de l'index dans la petite matrice avec le modulo (%)
			// j % 4 va donner : 0, 1, 2, 3, 0, 1, 2, 3, 0, 1...
			int index_src = (i * COL_ORIGINE) + (j % COL_ORIGINE);

			// Copie de la valeur
			large_L[index_dest] = L[index_src];
		}
	}

	int8_t L8[LIGNES*COL_CIBLE];


	simulationSDR::quantizer_transform8(large_L, L8, LIGNES*COL_CIBLE, 8, 4);

	uint8_t V_neon[LIGNES*COL_CIBLE];
	simulationSDR::codec_repetition_soft_decode8_neon(L8, V_neon, LIGNES*COL_CIBLE,3);
}

TEST(ReceiverTest, Monitor) {

	const uint8_t U[K] = {0, 1, 1, 0};
	const uint8_t V[K] = {1, 0, 1, 0};

	uint64_t fe = 0;
	uint64_t be = 0;

	simulationSDR::monitor_check_errors(U, U, K, &be, &fe);

	ASSERT_EQ(fe, 0) << "fe=" << static_cast<int>(fe);
	ASSERT_EQ(be, 0) << "be=" << static_cast<int>(be);

	simulationSDR::monitor_check_errors(U, V, K, &be, &fe);

	ASSERT_EQ(fe, 1) << "fe=" << static_cast<int>(fe);
	ASSERT_EQ(be, 2) << "be=" << static_cast<int>(be);
}

// TEST(ReceiverTest, Encode) {
// 	uint8_t u_k[K] = {0, 1, 1, 0};
// 	uint8_t c_n[N];

// 	size_t n_reps = N / K;

// 	codec_repetition_encode(u_k, c_n, K, n_reps);

// 	EXPECT_EQ(std::size(c_n), N);
// 	for (size_t k = 0; k < N; k++) {
// 		if (c_n[k] != u_k[k % K]) {
// 			FAIL() << "Encode error";
// 		}
// 	}
// }

// TEST(TransmitterTest, Modulate){

//     uint8_t c_n[N] = {0,1,1,0,0,1,1,0,0,1,1,0};
//     int32_t x_n[N];

//     modem_BPSK_modulate(c_n, x_n, N);

//     for (size_t k = 0; k < N; k++) {
//         if((c_n[k] == 0 && x_n[k] != 1) || c_n[k] == 1 && x_n[k] != -1){
//             FAIL();
//         }
// 	}
// }

} // namespace simulationSDR
