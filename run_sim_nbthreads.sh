#!/bin/bash
# Lance les 5 simulations multi avec -t 2,3,4,5 (enregistre sim.csv et simulation_stats.txt par run)
MAX_SNR_SIM1=10
MAX_SNR_OTHERS=10

THREADS=(2 3 4 5)

SIM_BIN=./build/bin/simulator

echo "Début de la campagne de simulations multithread..."

for t in "${THREADS[@]}"; do
  echo "=== Threads: $t ==="

  echo "Lancement de sim_neon_multi1 (-t $t)..."
  $SIM_BIN -t "$t" -m 0 -M "$MAX_SNR_SIM1" -s 1 -e 100 -K 32 -N 128 --neon-hard --qs 1 --qf 0
  [[ -f sim.csv ]] && mv sim.csv "sim_neon_multi1_t${t}.csv" || echo "sim.csv introuvable pour sim_neon_multi1_t${t}"
  [[ -f simulation_stats.txt ]] && mv simulation_stats.txt "simulation_stats_neon_multi1_t${t}.txt"
  echo "sim_neon_multi1_t${t} terminé"
  echo "---------------------------------------------------"

  echo "Lancement de sim_neon_multi2 (-t $t)..."
  $SIM_BIN -t "$t" -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 128 --neon-soft --qs 6 --qf 3
  [[ -f sim.csv ]] && mv sim.csv "sim_neon_multi2_t${t}.csv" || echo "sim.csv introuvable pour sim_neon_multi2_t${t}"
  [[ -f simulation_stats.txt ]] && mv simulation_stats.txt "simulation_stats_neon_multi2_t${t}.txt"
  echo "sim_neon_multi2_t${t} terminé"
  echo "---------------------------------------------------"

  echo "Lancement de sim_neon_multi3 (-t $t)..."
  $SIM_BIN -t "$t" -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 96 --neon-soft --qs 5 --qf 2
  [[ -f sim.csv ]] && mv sim.csv "sim_neon_multi3_t${t}.csv" || echo "sim.csv introuvable pour sim_neon_multi3_t${t}"
  [[ -f simulation_stats.txt ]] && mv simulation_stats.txt "simulation_stats_neon_multi3_t${t}.txt"
  echo "sim_neon_multi3_t${t} terminé"
  echo "---------------------------------------------------"

  echo "Lancement de sim_neon_multi4 (-t $t)..."
  $SIM_BIN -t "$t" -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 64 --neon-soft --qs 6 --qf 1
  [[ -f sim.csv ]] && mv sim.csv "sim_neon_multi4_t${t}.csv" || echo "sim.csv introuvable pour sim_neon_multi4_t${t}"
  [[ -f simulation_stats.txt ]] && mv simulation_stats.txt "simulation_stats_neon_multi4_t${t}.txt"
  echo "sim_neon_multi4_t${t} terminé"
  echo "---------------------------------------------------"

  echo "Lancement de sim_neon_multi5 (-t $t)..."
  $SIM_BIN -t "$t" -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 32 --neon-soft --qs 1 --qf 0
  [[ -f sim.csv ]] && mv sim.csv "sim_neon_multi5_t${t}.csv" || echo "sim.csv introuvable pour sim_neon_multi5_t${t}"
  [[ -f simulation_stats.txt ]] && mv simulation_stats.txt "simulation_stats_neon_multi5_t${t}.txt"
  echo "sim_neon_multi5_t${t} terminé"
  echo "---------------------------------------------------"

done

echo "Campagne multithread terminée."
