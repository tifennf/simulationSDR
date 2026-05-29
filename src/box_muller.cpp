// Alexander BAKALOV
// Tifenn FABRICI

#include "simulationSDR/box_muller.h"
#include "simulationSDR/neon_mathfun.h"

using namespace simulationSDR;

void simulationSDR::box_muller_neon(const float* u1_array, const float* u2_array,
                     float* z0_array, float* z1_array, size_t N) {

    float32x4_t v_epsilon = vdupq_n_f32(1e-7f);
    float32x4_t v_minus_two = vdupq_n_f32(-2.0f);
    float32x4_t v_two_pi = vdupq_n_f32(TWO_PI);

    // Traitement par bloc de 4 float32
    for (size_t i = 0; i + 3 < N; i += 4) {
        float32x4_t u1 = vld1q_f32(u1_array + i);
        float32x4_t u2 = vld1q_f32(u2_array + i);

        // on s'assure que ça ne soit pas à 0 pour pouvoir utiliser log
        u1 = vmaxq_f32(u1, v_epsilon);

        // calcul du rayon : r = sqrt(-2.0 * log(u1))
        float32x4_t log_u1 = log_ps(u1);                     // log(u1)
        float32x4_t r_sq = vmulq_f32(v_minus_two, log_u1);   // -2.0 * log(u1)

        float32x4_t r = vsqrtq_f32(r_sq);                    // sqrt(...)

        // calcul de l'angle : theta = 2 * PI * u2
        float32x4_t theta = vmulq_f32(v_two_pi, u2);

        float32x4_t cos_theta, sin_theta;
        sincos_ps(theta, &sin_theta, &cos_theta);

        // conversion coordonnées polaire -> cartésienne
        float32x4_t z0 = vmulq_f32(r, cos_theta);
        float32x4_t z1 = vmulq_f32(r, sin_theta);


        vst1q_f32(z0_array + i, z0);
        vst1q_f32(z1_array + i, z1);
    }

}
