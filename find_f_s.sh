#!/bin/bash

# --- VARIABLES ---
MAX_SNR_SIM1=15
MAX_SNR_OTHERS=12
# -----------------

echo "Début de la recherche de la meilleure combinaison pour --qf et --qs..."

# Boucle pour f de 0 à 8
for f in $(seq 0 8); do
  # Boucle pour s de 1 à 8
  for s in $(seq 1 8); do
    
    # Condition : s doit être strictement supérieur à f
    if [ $s -gt $f ]; then
      echo "---------------------------------------------------"
      echo "Test de la combinaison : f=$f, s=$s"
      echo "---------------------------------------------------"

      # --- SIMULATION 1 ---
      echo " Lancement de sim1 avec f=$f s=$s..."
      ./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_SIM1" -s 1 -e 100 -K 32 -N 128 --neon-hard --qf "$f" --qs "$s"
      mv sim.csv "sim1_f${f}_s${s}.csv"
      echo " sim1 (f=$f, s=$s) terminé."
      echo "-----------------"

      # --- SIMULATION 2 ---
      echo " Lancement de sim2 avec f=$f s=$s..."
      ./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 128 --neon-soft --qf "$f" --qs "$s"
      mv sim.csv "sim2_f${f}_s${s}.csv"
      echo " sim2 (f=$f, s=$s) terminé."
      echo "-----------------"

      # --- SIMULATION 3 ---
      echo " Lancement de sim3 avec f=$f s=$s..."
      ./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 96 --neon-soft --qf "$f" --qs "$s"
      mv sim.csv "sim3_f${f}_s${s}.csv"
      echo " sim3 (f=$f, s=$s) terminé."
      echo "-----------------"

      # --- SIMULATION 4 ---
      echo " Lancement de sim4 avec f=$f s=$s..."
      ./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 64 --neon-soft --qf "$f" --qs "$s"
      mv sim.csv "sim4_f${f}_s${s}.csv"
      echo " sim4 (f=$f, s=$s) terminé."
      echo "-----------------"

      # --- SIMULATION 5 ---
      echo " Lancement de sim5 avec f=$f s=$s..."
      ./build/bin/simulator -t 6 -m 0 -M "$MAX_SNR_OTHERS" -s 1 -e 100 -K 32 -N 32 --neon-soft --qf "$f" --qs "$s"
      mv sim.csv "sim5_f${f}_s${s}.csv"
      echo " sim5 (f=$f, s=$s) terminé."
      echo "-----------------"

    fi
  done
done

echo "---------------------------------------------------"
echo "Toutes les combinaisons ont été testées avec succès !"
echo "---------------------------------------------------"
