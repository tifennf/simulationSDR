// Alexander BAKALOV
// Tifenn FABRICI

#pragma once
#include <cmath>
#include <arm_neon.h>
#include <cstddef>

namespace simulationSDR{
    constexpr float TWO_PI = 6.283185307179586f;

    void box_muller_neon(const float* u1_array, const float* u2_array,
                         float* z0_array, float* z1_array, size_t N);

}
