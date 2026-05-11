#!/bin/bash

# --- VALIDATION DES ARGUMENTS ---
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <f> <s>"
    exit 1
fi

f=$1
s=$2

if ! [[ "$f" =~ ^[0-9]+$ ]] || [ "$f" -lt 0 ] || [ "$f" -gt 8 ]; then
    echo "Erreur: La valeur de 'f' doit être un entier entre 0 et 8."
    exit 1
fi

if ! [[ "$s" =~ ^[0-9]+$ ]] || [ "$s" -lt 1 ] || [ "$s" -gt 8 ]; then
    echo "Erreur: La valeur de 's' doit être un entier entre 1 et 8."
    exit 1
fi

if [ "$s" -le "$f" ]; then
    echo "Erreur: La valeur de 's' ($s) doit être strictement supérieure à celle de 'f' ($f)."
    exit 1
fi

echo "Simulations de validation d'implémentation hard"
# --- SIMULATION HARD ---
echo "Lancement de sim hard..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-hard"
mv sim.csv sim_hard.csv
echo "sim hard terminé et sauvgardée dans sim_hard.csv"
echo "---------------------------------------------------"

# --- SIMULATION HARD 8 ---
echo "Lancement de sim hard 8..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-hard8"      --qf "$f" --qs "$s" 
mv sim.csv sim_hard_8.csv
echo "sim hard 8 terminé et sauvgardée dans sim_hard_8.csv"
echo "---------------------------------------------------"

# --- SIMULATION HARD 8 NEON ---
echo "Lancement de sim hard 8 neon..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-hard8-neon" --qf "$f" --qs "$s"
mv sim.csv sim_hard_8_neon.csv
echo "sim hard 8 neon terminé et sauvgardée dans sim_hard_8_neon.csv"
echo "---------------------------------------------------"

echo "---------------------------------------------------"
echo "Simulations de validation d'implémentation soft"
# --- SIMULATION SOFT ---
echo "Lancement de sim soft..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-soft"
mv sim.csv sim_soft.csv
echo "sim soft terminé et sauvgardée dans sim_soft.csv"
echo "---------------------------------------------------"

# --- SIMULATION SOFT 8 ---
echo "Lancement de sim soft 8..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-soft8"      --qf "$f" --qs "$s"
mv sim.csv sim_soft_8.csv
echo "sim soft 8 terminé et sauvgardée dans sim_soft_8.csv"
echo "---------------------------------------------------"

# --- SIMULATION SOFT 8 NEON ---
echo "Lancement de sim soft 8 neon..."
./simulator -m 0 -M 14 -s 1 -e 100 -K 32 -N 8192 -D "rep-soft8-neon" --qf "$f" --qs "$s"
mv sim.csv sim_soft_8_neon.csv
echo "sim soft 8 neon terminé et sauvgardée dans sim_soft_8_neon.csv"
