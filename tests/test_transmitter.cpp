#include "gtest/gtest.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <iterator>

#include "simulationSDR/transmitter.h"

#define K 4
#define N 3*K

namespace simulationSDR {

TEST(TransmitterTest, SourceGenerate) {

	uint8_t u_k[K];
	source_generate(u_k, K);

	EXPECT_EQ(std::size(u_k), K) << "Wrong size";
	for (uint8_t e : u_k) {
		if (e != 0 && e != 1) {
			FAIL() << "Not a binary vector";
		}
	}
}

TEST(TransmitterTest, Encode) {
	uint8_t u_k[K] = {0, 1, 1, 0};
	uint8_t c_n[N];

	size_t n_reps = N / K;

	codec_repetition_encode(u_k, c_n, K, n_reps);

	EXPECT_EQ(std::size(c_n), N);
	for (size_t k = 0; k < N; k++) {
		if (c_n[k] != u_k[k % K]) {
			FAIL() << "Encode error";
		}
	}
}

TEST(TransmitterTest, Modulate){

    uint8_t c_n[N] = {0,1,1,0,0,1,1,0,0,1,1,0};
    int32_t x_n[N];

    modem_BPSK_modulate(c_n, x_n, N);

    for (size_t k = 0; k < N; k++) {
        if((c_n[k] == 0 && x_n[k] != 1) || c_n[k] == 1 && x_n[k] != -1){
            FAIL();
        }
	}
}

} // namespace simulationSDR
