#!/usr/bin/env python3

import os
import re
import glob
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import seaborn as sns

# 1. Récupération de tous les fichiers CSV correspondants
files = glob.glob("sim_neon_multi*_t*.csv")

if not files:
    print("Erreur : Aucun fichier trouvé. Lancez le script dans le dossier contenant les CSV.")
    exit()

all_data = []

# 2. Analyse des fichiers et extraction des données
for file in files:
    # On extrait le nom de la config (ex: multi1) et le nombre de threads (ex: 5) via regex
    match = re.search(r"(sim_neon_multi\d+)_t(\d+)\.csv", file)
    if match:
        config = match.group(1)
        threads = int(match.group(2))
        
        # Lecture du CSV (séparateur ';')
        df = pd.read_csv(file, sep=';')
        
        # On calcule le throughput moyen sur l'ensemble de la simulation
        # Note : Si tes plages de SNR diffèrent entre les fichiers, 
        # il vaudrait mieux filtrer sur un SNR précis. Ici on prend la moyenne globale.
        mean_throughput = df['sim_thr'].mean()
        
        all_data.append({
            'config': config,
            'threads': threads,
            'vitesse_moyenne': mean_throughput
        })

# Création du DataFrame global
df_results = pd.DataFrame(all_data)

# Tri pour obtenir un affichage et des lignes de graphiques continues et propres
df_results = df_results.sort_values(by=['config', 'threads'])

# 3. Affichage des résultats en console
print("--- RÉSUMÉ DES PERFORMANCES ---")
print(df_results.to_string(index=False))

# 4. Génération du graphique de comparaison
plt.figure(figsize=(10, 6))
sns.set_theme(style="whitegrid")

# On trace une courbe par configuration (multi1, multi2, etc.)
sns.lineplot(
    data=df_results, 
    x='threads', 
    y='vitesse_moyenne', 
    hue='config', 
    marker='o', 
    linewidth=2.5, 
    markersize=8
)

# Personnalisation du graphique
plt.title("Évolution de la vitesse de simulation selon le nombre de threads", fontsize=14, fontweight='bold')
plt.xlabel("Nombre de threads (t)", fontsize=12)
plt.ylabel("Throughput Moyen (sim_thr)", fontsize=12)

# Forcer l'affichage de tous les threads sur l'axe X (évite les demi-threads en float)
plt.xticks(df_results['threads'].unique())

plt.legend(title="Configuration")
plt.tight_layout()

# Sauvegarde et affichage
output_image = "comparaison_vitesse_threads.png"
plt.savefig(output_image, dpi=300)
print(f"\n[Succès] Le graphique a été sauvegardé sous le nom : '{output_image}'")
plt.show()
