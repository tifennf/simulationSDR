#!/usr/bin/env python3

import os
import re
import glob
import pandas as pd

# Désactivation de l'affichage de fenêtres Qt (évite le warning Wayland)
import matplotlib
matplotlib.use('Agg') 
import matplotlib.pyplot as plt
import seaborn as sns

# 1. Recherche des fichiers de statistiques (.txt)
files = glob.glob("simulation_stats_neon_multi*_t*.txt")

if not files:
    print("Erreur : Aucun fichier de stats trouvé (ex: simulation_stats_neon_multi1_t2.txt).")
    exit()

all_data = []

# 2. Parsing des fichiers de profils
for file in files:
    # Extraction du nombre de threads depuis le nom du fichier
    match = re.search(r"_t(\d+)\.txt", file)
    if match:
        threads = int(match.group(1))
        
        with open(file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # Nettoyage des sauts de ligne ou balises parasites pour un parsing robuste
        content = re.sub(r'\\', '', content)
        content = re.sub(r'\s+', ' ', content)
        
        # Regex pour capturer : Nom de l'étape : Avg, Min, Max, Mbps, Pourcentage %
        pattern = r"(Temps [\wéè]+)\s*:\s*([0-9.]+),\s*([0-9.]+),\s*([0-9.]+),\s*([0-9.]+),\s*([0-9.]+)\s*%"
        matches = re.findall(pattern, content)
        
        for row in matches:
            stage_name = row[0].replace("Temps ", "").strip() # On garde juste 'AWGN', 'Décodage', etc.
            pct_temps = float(row[5])
            
            all_data.append({
                'threads': threads,
                'etape': stage_name,
                'pourcentage': pct_temps
            })

# Création du DataFrame
df = pd.DataFrame(all_data)

# On fait la moyenne des pourcentages par étape et par configuration de threads 
# (au cas où tu as plusieurs multi1, multi2 pour un même nombre de threads)
df_grouped = df.groupby(['threads', 'etape'])['pourcentage'].mean().reset_index()

# Pivoter le tableau pour avoir les étapes en colonnes (prêt pour le graphique empilé)
df_pivot = df_grouped.pivot(index='threads', columns='etape', values='pourcentage')

# S'assurer que le total par thread fait bien ~100% pour l'affichage cohérent
df_pivot = df_pivot.div(df_pivot.sum(axis=1), axis=0) * 100

# 3. Génération du graphique en barres empilées
plt.figure(figsize=(12, 7))
sns.set_theme(style="whitegrid")

# Tracé des barres empilées
df_pivot.plot(kind='bar', stacked=True, cmap='tab10', ax=plt.gca(), width=0.6)

# Personnalisation
plt.title("Évolution du profil d'exécution (Profil de charge) selon le nombre de threads", fontsize=14, fontweight='bold')
plt.xlabel("Nombre de threads (t)", fontsize=12)
plt.ylabel("Répartition du temps de calcul (%)", fontsize=12)
plt.xticks(rotation=0)
plt.ylim(0, 100)

# Positionnement de la légende à l'extérieur
plt.legend(title="Étapes de la simulation", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()

# Sauvegarde
output_image = "profil_execution_vs_threads.png"
plt.savefig(output_image, dpi=300)
print(f"[Succès] Le graphique d'analyse de profil a été sauvegardé sous : '{output_image}'")
