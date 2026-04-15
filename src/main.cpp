#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <CLI/CLI.hpp>

#include "simulationSDR/receiver.h"
#include "simulationSDR/transmitter.h"
#include "simulationSDR/stats.h"

using namespace simulationSDR;

// snr : signal to noise ratio
// esn : energie to signal noise

int main(int argc, char** argv) {
	CLI::App app("Simulateur Monte Carlo");

	/* Declaration des options
	 * Declaration de la variable avec sa valeur par défaut
	 * Ajout de l'option à l'application
	 * Verification optionnelle
	 */

	float min_SNR = 0;
	app.add_option("-m", min_SNR, "Min SNR");
	float max_SNR = 0;
	app.add_option("-M", max_SNR, "Max SNR");

	float step_val = 1;
	app.add_option("-s", step_val, "Step between two SNR points");

	uint32_t f_max = 100;
	app.add_option("-e", f_max, "Number of frame errors to reach to explore one SNR point");

	size_t K = 4;
	app.add_option("-K", K, "Number of information bits");
	size_t N = 12;
	app.add_option("-N", N, "Codeword size");

	std::string rep = "rep-hard";
	app.add_option("-D", rep, "Decoder type")
		->check(CLI::IsMember(
			{"rep-hard", "rep-soft", "rep-hard8", "rep-soft8"})); // Verifie que la valeur est l'une de ces options

        bool src_all_zeros = false;
        app.add_flag("--src-all-zeros", src_all_zeros, "Generate frames containing only zeros");

        bool mod_all_ones = false;
        app.add_flag("--mod-all-ones", mod_all_ones, "Skip source execution and set modulation output to only ones");

        size_t f = 0;
        size_t s = 8;
        auto *opt_qf = app.add_option("--qf", f, "Enable quantizer and set the number of bits of the fractional part")
                ->check(CLI::Range(0, 8));
        app.add_option("--qs", s, "Number of bits used in the quantizer block")
                ->check(CLI::Range(1, 8))
                ->needs(opt_qf); // Vérifie que --qf est présent si --qs est présent

	CLI11_PARSE(app, argc, argv);

        // Verifications apres le parsing
	if (N % K) {
	    std::cerr << "N must be a multiple of K" << std::endl;
	    return 1;
	}

        const bool quant_enabled = (opt_qf->count() > 0);
        if (quant_enabled) {
            if (s < f) {
                std::cerr << "s must be superior or equal to f" << std::endl;
                return 1;
            }
        }

	/* ---------- Simulation ---------- */

        /* Output */
	std::ofstream file("sim.csv");

	if (!file.is_open()) {
		std::cerr << "Erreur : Impossible de créer le fichier CSV !" << std::endl;
		return 1;
	}

	file << "snr_bit;snr_symbol;sigma;be;fe;frame_cpt;ber;fer;time;frame_avg_time\n";

        /*
         * Tableaux de chaque block
	 * Declaration dynamique des tableaux car taille non constante
         */
	uint8_t* u_k = new uint8_t[K];
	uint8_t* c_n = new uint8_t[N];
	int32_t* x_n = new int32_t[N];
	float* y_n = new float[N];
	float* l_n = new float[N];
	uint8_t* v_k = new uint8_t[K];

        /* Variables de simulation Lab 3 */
	size_t n_reps = N / K;
	float R = (float)K / N;

	float snr_symbol, sigma;
	uint64_t n_bit_errors, n_frame_errors;
	uint64_t sim_frame_cpt;

	float bit_error_rate, frame_error_rate;

        /* Variables de simulation Lab 4 */
        float sim_thr;
        float avg_lat = 1, min_lat = 1, max_lat = 1, thr = 1, per = 1;

        /* Boucle de la simulation */
	for (float snr_bit = min_SNR; snr_bit <= max_SNR; snr_bit += step_val) {

		auto start = std::chrono::high_resolution_clock::now();

		n_bit_errors = 0;
		n_frame_errors = 0;
		sim_frame_cpt = 0;

		snr_symbol = snr_bit + 10 * log10(R); // bs = 1 here (BPSK)
		sigma = sqrt(1 / (2 * pow(10, snr_symbol / 10)));

		do {
			// generate a new trame
                        source_generate(u_k, K);
                        stats_avg_latence_block(&avg_lat);
                        stats_max_latence_block(&max_lat);
                        stats_min_latence_block(&min_lat);
                        stats_throughput_block(&thr);
                        stats_percentage_block(&per);

			codec_repetition_encode(u_k, c_n, K, n_reps);

			modem_BPSK_modulate(c_n, x_n, N);

			channel_AWGN_add_noise(x_n, y_n, N, sigma);

			modem_BPSK_demodulate(y_n, l_n, N, sigma);

			if (rep == "rep-hard") {
				codec_repetition_hard_decode(l_n, v_k, K, n_reps);
			} else {
				codec_repetition_soft_decode(l_n, v_k, K, n_reps);
			}

			monitor_check_errors(u_k, v_k, K, &n_bit_errors, &n_frame_errors);

			sim_frame_cpt++;

		} while (n_frame_errors < f_max);

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		double time_spent = duration.count();
		double avg_time = time_spent / sim_frame_cpt;

		bit_error_rate = (float)n_bit_errors / (sim_frame_cpt * K);
		frame_error_rate = (float)n_frame_errors / sim_frame_cpt;

                sim_thr = (sim_frame_cpt * K) / time_spent;

		file << snr_bit << ";" << snr_symbol << ";" << sigma << ";" << n_bit_errors << ";"
			 << n_frame_errors << ";" << sim_frame_cpt << ";" << bit_error_rate << ";"
			 << frame_error_rate << ";" << time_spent << ";" << avg_time << "\n";

                std::cout << "Sim_thr: " << sim_thr
                  << " avg_lat: " << avg_lat
                  << " max_lat: " << max_lat
                  << " min_lat: " << min_lat
                  << " throughput: " << thr
                  << " percentage: " << per
                  << std::endl;

		std::cout << "SNR: " << snr_bit << " terminé." << std::endl;
	}

        /* Nettoyage mémoire */
	delete[] u_k;
	delete[] c_n;
	delete[] x_n;
	delete[] y_n;
	delete[] l_n;
	delete[] v_k;

	return 0;
}
