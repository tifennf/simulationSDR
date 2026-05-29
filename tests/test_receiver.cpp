#include "gtest/gtest.h"
#include <cstdint>
#include <gtest/gtest.h>

#include "simulationSDR/receiver.h"
#include <cmath>



namespace simulationSDR {

TEST(ReceiverTest, AWGN_Neon_ZeroNoise) {
    size_t N = 1000;
    std::vector<int32_t> signal_in(N, 42); // Signal constant à 42
    std::vector<float> signal_out(N, 0.0f);

    // On applique un bruit de 0
    simulationSDR::channel_AWGN_add_noise_neon(signal_in.data(), signal_out.data(), N, 0.0f);

    for (size_t i = 0; i < N; i++) {
        EXPECT_FLOAT_EQ(signal_out[i], 42.0f);
    }
}


TEST(ReceiverTest, AWGN_Neon_StatisticalValidity) {
    size_t N = 20000;
    std::vector<int32_t> signal_in(N, 0);
    std::vector<float> signal_out(N, 0.0f);
    float target_sigma = 2.5f;

    simulationSDR::channel_AWGN_add_noise_neon(signal_in.data(), signal_out.data(), N, target_sigma);

    double sum = 0.0;
    double sum_sq = 0.0;

    for (size_t i = 0; i < N; i++) {
        sum += signal_out[i];
        sum_sq += signal_out[i] * signal_out[i];
    }

    double mean = sum / N;
    double variance = (sum_sq / N) - (mean * mean);
    double stddev = std::sqrt(variance);

    EXPECT_NEAR(mean, 0.0, 0.1) << "La moyenne doit être proche de 0";
    EXPECT_NEAR(stddev, target_sigma, 0.1) << "L'écart-type doit être proche du sigma demandé";
}


TEST(ReceiverTest, HardDecode) {
    {
        const size_t K = 4;
        const size_t n_reps = 3;
        const size_t N = K * n_reps;
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
}

TEST(ReceiverTest, SoftDecode) {
    {
        const size_t K = 4;
        const size_t n_reps = 3;
        const size_t N = K * n_reps;
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
    }

}

TEST(ReceiverTest, Monitor) {
    const size_t K = 4;

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

TEST(ReceiverTest, MonitorNeon){
    const size_t K = 16;

	uint8_t U[K] = {0, 1, 1, 0};
	uint8_t V[K] = {1, 0, 1, 0};

	for(int i =4; i<16; i++){
	    U[i] = U[i%4];
	    V[i] = V[i%4];
	}

	uint64_t fe = 0;
	uint64_t be = 0;

	simulationSDR::monitor_check_errors_neon(U, U, K, &be, &fe);

	ASSERT_EQ(fe, 0) << "fe=" << static_cast<int>(fe);
	ASSERT_EQ(be, 0) << "be=" << static_cast<int>(be);

	simulationSDR::monitor_check_errors_neon(U, V, K, &be, &fe);

	ASSERT_EQ(fe, 1) << "fe=" << static_cast<int>(fe);
	ASSERT_EQ(be, 8) << "be=" << static_cast<int>(be);
}

TEST(QuantizerTest, Transform8_BasicAndSaturation) {
    const size_t N = 5;
    // On prend un panel de valeurs flottantes (positives, négatives, à virgule, et très grandes)
    float L_N[N] = {1.0f, -1.5f, 0.1f, 10.0f, -10.0f};
    int8_t L8_N[N] = {0};

    size_t s = 8;
    size_t f = 4;

    simulationSDR::quantizer_transform8(L_N, L8_N, N, s, f);

    // Vérifications :
    // 1.0 * 16 = 16 (Valeur classique)
    EXPECT_EQ(L8_N[0], 16) << "Erreur sur la valeur classique";

    // -1.5 * 16 = -24 (Valeur négative)
    EXPECT_EQ(L8_N[1], -24) << "Erreur sur la valeur négative";

    // 0.1 * 16 = 1.6 -> arrondi à 2 (Test de l'arrondi)
    EXPECT_EQ(L8_N[2], 2) << "Erreur sur l'arrondi (round)";

    // 10.0 * 16 = 160 -> Dépasse 127, donc sature à 127 (Test Saturation Max)
    EXPECT_EQ(L8_N[3], 127) << "Erreur de saturation maximale";

    // -10.0 * 16 = -160 -> Dépasse -128, donc sature à -128 (Test Saturation Min)
    EXPECT_EQ(L8_N[4], -128) << "Erreur de saturation minimale";
}

TEST(QuantizerTest, Transform8_CustomBitwidth) {
    const size_t N = 3;
    float L_N[N] = {5.0f, 10.0f, -10.0f};
    int8_t L8_N[N] = {0};


    size_t s = 6;
    size_t f = 2;

    simulationSDR::quantizer_transform8(L_N, L8_N, N, s, f);

    // 5.0 * 4 = 20
    EXPECT_EQ(L8_N[0], 20);

    // 10.0 * 4 = 40 -> Dépasse 31, sature à 31
    EXPECT_EQ(L8_N[1], 31) << "Erreur de saturation Max pour s=6";

    // -10.0 * 4 = -40 -> Dépasse -32, sature à -32
    EXPECT_EQ(L8_N[2], -32) << "Erreur de saturation Min pour s=6";
}

} // namespace simulationSDR
