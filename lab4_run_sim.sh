#!/bin/bash

# --- VARIABLES ---
MAX_SNR_SIM1=15
MAX_SNR_OTHERS=12
# -----------------

echo "Début de la campagne de simulations..."

echo "---------------------------------------------------"
echo "Simulations avec source générée à que des zeros (--src-all-zeros)"
echo "---------------------------------------------------"

# --- SIMULATION 1 (ZEROS) ---
echo " Lancement de sim1 (zeros)..."
./build/bin/simulator -m 0 -M $MAX_SNR_SIM1 -s 1 -e 100 -K 32 -N 128 -D "rep-hard" --src-all-zeros
mv sim.csv sim1_zeros.csv
echo "sim1 (zeros) terminé et sauvegardé dans sim1_zeros.csv"
echo "---------------------------------------------------"

# --- SIMULATION 2 (ZEROS) ---
echo "Lancement de sim2 (zeros)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 128 -D "rep-soft" --src-all-zeros
mv sim.csv sim2_zeros.csv
echo "sim2 (zeros) terminé et sauvegardé dans sim2_zeros.csv"
echo "---------------------------------------------------"

# --- SIMULATION 3 (ZEROS) ---
echo "Lancement de sim3 (zeros)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 96 -D "rep-soft" --src-all-zeros
mv sim.csv sim3_zeros.csv
echo "sim3 (zeros) terminé et sauvegardé dans sim3_zeros.csv"
echo "---------------------------------------------------"

# --- SIMULATION 4 (ZEROS) ---
echo "Lancement de sim4 (zeros)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 64 -D "rep-soft" --src-all-zeros
mv sim.csv sim4_zeros.csv
echo "sim4 (zeros) terminé et sauvegardé dans sim4_zeros.csv"
echo "---------------------------------------------------"

# --- SIMULATION 5 (ZEROS) ---
echo "Lancement de sim5 (zeros)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 32 -D "rep-soft" --src-all-zeros
mv sim.csv sim5_zeros.csv
echo "sim5 (zeros) terminé et sauvegardé dans sim5_zeros.csv"
echo "---------------------------------------------------"


echo "---------------------------------------------------"
echo "Simulations avec modulation à que des 1 (--mod-all-ones)"
echo "---------------------------------------------------"

# --- SIMULATION 1 (ONES) ---
echo " Lancement de sim1 (ones)..."
./build/bin/simulator -m 0 -M $MAX_SNR_SIM1 -s 1 -e 100 -K 32 -N 128 -D "rep-hard" --mod-all-ones
mv sim.csv sim1_ones.csv
echo "sim1 (ones) terminé et sauvegardé dans sim1_ones.csv"
echo "---------------------------------------------------"

# --- SIMULATION 2 (ONES) ---
echo "Lancement de sim2 (ones)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 128 -D "rep-soft" --mod-all-ones
mv sim.csv sim2_ones.csv
echo "sim2 (ones) terminé et sauvegardé dans sim2_ones.csv"
echo "---------------------------------------------------"

# --- SIMULATION 3 (ONES) ---
echo "Lancement de sim3 (ones)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 96 -D "rep-soft" --mod-all-ones
mv sim.csv sim3_ones.csv
echo "sim3 (ones) terminé et sauvegardé dans sim3_ones.csv"
echo "---------------------------------------------------"

# --- SIMULATION 4 (ONES) ---
echo "Lancement de sim4 (ones)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 64 -D "rep-soft" --mod-all-ones
mv sim.csv sim4_ones.csv
echo "sim4 (ones) terminé et sauvegardé dans sim4_ones.csv"
echo "---------------------------------------------------"

# --- SIMULATION 5 (ONES) ---
echo "Lancement de sim5 (ones)..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 32 -D "rep-soft" --mod-all-ones
mv sim.csv sim5_ones.csv
echo "sim5 (ones) terminé et sauvegardé dans sim5_ones.csv"
echo "---------------------------------------------------"

echo "Toutes les simulations sont terminées avec succès !"
