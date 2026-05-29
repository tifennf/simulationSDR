#!/bin/bash

# --- VARIABLES ---
MAX_SNR_SIM1=15
MAX_SNR_OTHERS=12
# -----------------

echo "Début de la campagne de simulations..."

# --- SIMULATION 1 ---
echo "Lancement de sim_neon_multi1 (-t 6)..."
./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_SIM1" -s 1 -e 100 -K 32 -N 128 --neon-hard --qs 1 --qf 0
[[ -f sim.csv ]] && mv sim.csv sim_neon_multi1.csv || echo "sim.csv introuvable pour sim_neon_multi1"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon_multi1.txt
echo "sim_neon_multi1 terminé"
echo "Lancement de sim_neon1 (sans -t)..."
./build/bin/simulator -m 0 -M "$MAX_SNR_SIM1" -s 1 -e 100 -K 32 -N 128 --neon-hard --qs 1 --qf 0
[[ -f sim.csv ]] && mv sim.csv sim_neon1.csv || echo "sim.csv introuvable pour sim_neon1"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon1.txt
echo "sim_neon1 terminé"
echo "---------------------------------------------------"

# --- SIMULATION 2 ---
echo "Lancement de sim_neon_multi2 (-t 6)..."
./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 128 --neon-soft --qs 6 --qf 3
[[ -f sim.csv ]] && mv sim.csv sim_neon_multi2.csv || echo "sim.csv introuvable pour sim_neon_multi2"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon_multi2.txt
echo "sim_neon_multi2 terminé"
echo "Lancement de sim_neon2 (sans -t)..."
./build/bin/simulator -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 128 --neon-soft --qs 6 --qf 3
[[ -f sim.csv ]] && mv sim.csv sim_neon2.csv || echo "sim.csv introuvable pour sim_neon2"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon2.txt
echo "sim_neon2 terminé"
echo "---------------------------------------------------"

# --- SIMULATION 3 ---
echo "Lancement de sim_neon_multi3 (-t 6)..."
./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 96 --neon-soft --qs 5 --qf 2
[[ -f sim.csv ]] && mv sim.csv sim_neon_multi3.csv || echo "sim.csv introuvable pour sim_neon_multi3"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon_multi3.txt
echo "sim_neon_multi3 terminé"
echo "Lancement de sim_neon3 (sans -t)..."
./build/bin/simulator -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 96 --neon-soft --qs 5 --qf 2
[[ -f sim.csv ]] && mv sim.csv sim_neon3.csv || echo "sim.csv introuvable pour sim_neon3"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon3.txt
echo "sim_neon3 terminé"
echo "---------------------------------------------------"

# --- SIMULATION 4 ---
echo "Lancement de sim_neon_multi4 (-t 6)..."
./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 64 --neon-soft --qs 6 --qf 1
[[ -f sim.csv ]] && mv sim.csv sim_neon_multi4.csv || echo "sim.csv introuvable pour sim_neon_multi4"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon_multi4_snr.txt
echo "sim_neon_multi4 terminé"
echo "Lancement de sim_neon4 (sans -t)..."
./build/bin/simulator -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 64 --neon-soft --qs 6 --qf 1
[[ -f sim.csv ]] && mv sim.csv sim_neon4_snr.csv || echo "sim.csv introuvable pour sim_neon4"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon4_snr.txt
echo "sim_neon4 terminé"
echo "---------------------------------------------------"

# --- SIMULATION 5 ---
echo "Lancement de sim_neon_multi5 (-t 6)..."
./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 32 --neon-soft --qs 1 --qf 0
[[ -f sim.csv ]] && mv sim.csv sim_neon_multi5.csv || echo "sim.csv introuvable pour sim_neon_multi5"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon_multi5.txt
echo "sim_neon_multi5 terminé"
echo "Lancement de sim_neon5 (sans -t)..."
./build/bin/simulator -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 32 --neon-soft --qs 1 --qf 0
[[ -f sim.csv ]] && mv sim.csv sim_neon5.csv || echo "sim.csv introuvable pour sim_neon5"
[[ -f simulation_stats.txt ]] && mv simulation_stats.txt simulation_stats_neon5.txt
echo "sim_neon5 terminé"
echo "---------------------------------------------------"

