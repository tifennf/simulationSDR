#include "simulationSDR/box_muller.h"
#include "simulationSDR/neon_mathfun.h"

using namespace simulationSDR;

void simulationSDR::box_muller_neon(const float* u1_array, const float* u2_array,
                     float* z0_array, float* z1_array, size_t N) {

    size_t i = 0;

    // Constantes vectorisées chargées une seule fois en dehors de la boucle
    float32x4_t v_epsilon = vdupq_n_f32(1e-7f);
    float32x4_t v_minus_two = vdupq_n_f32(-2.0f);
    float32x4_t v_two_pi = vdupq_n_f32(TWO_PI);

    // --- ÉTAPE 1 : Traitement vectorisé par blocs de 4 ---
    for (; i + 3 < N; i += 4) {
        // 1. Charger 4 valeurs de u1 et u2
        float32x4_t u1 = vld1q_f32(u1_array + i);
        float32x4_t u2 = vld1q_f32(u2_array + i);

        // 2. Protection contre le zéro (équivalent de: if (u1 <= 0) u1 = 1e-7)
        u1 = vmaxq_f32(u1, v_epsilon);

        // 3. Calcul du rayon : r = sqrt(-2.0 * log(u1))
        // Remplacement par la fonction de neon_mathfun.h
        float32x4_t log_u1 = log_ps(u1);                     // log(u1)
        float32x4_t r_sq = vmulq_f32(v_minus_two, log_u1);       // -2.0 * log(u1)

        // Note: vsqrtq_f32 nécessite une architecture ARMv8 (AArch64 - 64 bits).
        float32x4_t r = vsqrtq_f32(r_sq);                        // sqrt(...)

        // 4. Calcul de l'angle : theta = 2 * PI * u2
        float32x4_t theta = vmulq_f32(v_two_pi, u2);

        // 5. Trigonométrie OPTIMISÉE
        // On utilise sincos_ps pour calculer le cosinus et le sinus simultanément
        float32x4_t cos_theta, sin_theta;
        sincos_ps(theta, &sin_theta, &cos_theta);

        // 6. Conversion polaire -> cartésien
        float32x4_t z0 = vmulq_f32(r, cos_theta);
        float32x4_t z1 = vmulq_f32(r, sin_theta);

        // 7. Stocker les 4 résultats en mémoire
        vst1q_f32(z0_array + i, z0);
        vst1q_f32(z1_array + i, z1);
    }

    // // --- ÉTAPE 2 : Gérer la traîne (si N n'est pas un multiple de 4) ---
    // // On repasse en scalaire classique pour les 1, 2 ou 3 derniers éléments
    // for (; i < N; i++) {
    //     float u1 = u1_array[i];
    //     float u2 = u2_array[i];

    //     if (u1 <= 0.0f) u1 = 1e-7f;

    //     float r = std::sqrt(-2.0f * std::log(u1));
    //     float theta = TWO_PI * u2;

    //     z0_array[i] = r * std::cos(theta);
    //     z1_array[i] = r * std::sin(theta);
    // }
}
