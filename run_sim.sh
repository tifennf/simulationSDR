#!/bin/bash

# --- VARIABLES ---
MAX_SNR_SIM1=15
MAX_SNR_OTHERS=12
# -----------------

echo "🚀 Début de la campagne de simulations..."

# --- SIMULATION 1 ---
echo "▶️ Lancement de sim1..."
./build/bin/simulator -m 0 -M $MAX_SNR_SIM1 -s 1 -e 100 -K 32 -N 128 -D "rep-hard"
mv sim.csv sim1.csv
echo "✅ sim1 terminé et sauvegardé dans sim1.csv"
echo "---------------------------------------------------"

# --- SIMULATION 2 ---
echo "▶️ Lancement de sim2..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 128 -D "rep-soft"
mv sim.csv sim2.csv
echo "✅ sim2 terminé et sauvegardé dans sim2.csv"
echo "---------------------------------------------------"

# --- SIMULATION 3 ---
echo "▶️ Lancement de sim3..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 96 -D "rep-soft"
mv sim.csv sim3.csv
echo "✅ sim3 terminé et sauvegardé dans sim3.csv"
echo "---------------------------------------------------"

# --- SIMULATION 4 ---
echo "▶️ Lancement de sim4..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 64 -D "rep-soft"
mv sim.csv sim4.csv
echo "✅ sim4 terminé et sauvegardé dans sim4.csv"
echo "---------------------------------------------------"

# --- SIMULATION 5 ---
echo "▶️ Lancement de sim5..."
./build/bin/simulator -m 0 -M $MAX_SNR_OTHERS -s 1 -e 100 -K 32 -N 32 -D "rep-soft"
mv sim.csv sim5.csv
echo "✅ sim5 terminé et sauvegardé dans sim5.csv"
echo "---------------------------------------------------"

echo "🎉 Toutes les simulations sont terminées avec succès !"
