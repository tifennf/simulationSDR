#pragma once

namespace simulationSDR {
  // write into the buffer U_K
  void source_generate(uint8_t *U_K, size_t K);
  // read from the buffer U_K and write into the buffer C_N
  void codec_repetition_encode(const uint8_t *U_K, uint8_t *C_N, size_t K, size_t n_reps);
  // read from C_N, write into X_N
  void modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N); 
}
