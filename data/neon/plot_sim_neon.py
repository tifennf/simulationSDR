import os

import matplotlib.pyplot as plt
import pandas as pd


def charger_csv(fichier):
    """Charge le CSV en utilisant le point-virgule comme séparateur."""
    if not os.path.exists(fichier):
        print(f"Fichier {fichier} introuvable.")
        return None
    try:
        # On lit le CSV en précisant que le séparateur est ';'
        df = pd.read_csv(fichier, sep=";")
        # Petite sécurité : on supprime les espaces éventuels autour des noms de colonnes
        df.columns = df.columns.str.strip()
        return df
    except Exception as e:
        print(f"Erreur de lecture pour {fichier} : {e}")
        return None


# ==========================================
# Graphe 1 : sim_neon1 vs sim_neon2 (Hard vs Soft)
# ==========================================
plt.figure(figsize=(10, 6))
plt.yscale("log")
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.xlabel("Signal-to-Noise Ratio ($E_b/N_0$) (dB)")
plt.ylabel("Frame/Bit Error Rate")
plt.title("Hard Input Decoder VS Soft Input Decoder")

df1 = charger_csv("sim_neon1.csv")
df2 = charger_csv("sim_neon2.csv")

if df1 is not None and not df1.empty:
    plt.plot(
        df1["snr_bit"],
        df1["fer"],
        marker="x",
        linestyle="-",
        color="tab:green",
        label="FER sim_neon1 (Hard)",
    )
    plt.plot(
        df1["snr_bit"],
        df1["ber"],
        marker="o",
        linestyle="--",
        color="tab:green",
        fillstyle="none",
        label="BER sim_neon1 (Hard)",
    )
if df2 is not None and not df2.empty:
    plt.plot(
        df2["snr_bit"],
        df2["fer"],
        marker="x",
        linestyle="-",
        color="tab:blue",
        label="FER sim_neon2 (Soft)",
    )
    plt.plot(
        df2["snr_bit"],
        df2["ber"],
        marker="o",
        linestyle="--",
        color="tab:blue",
        fillstyle="none",
        label="BER sim_neon2 (Soft)",
    )

plt.legend()
plt.tight_layout()
# 1. Sauvegarde du premier graphique en PNG
plt.savefig("graph_1_hard_vs_soft.png", dpi=300)
plt.show()

# ==========================================
# Graphe 2 : sim_neon2 à sim_neon5 (Différents Code Rates en fonction de snr_bit)
# ==========================================
plt.figure(figsize=(10, 6))
plt.yscale("log")
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.xlabel("Signal-to-Noise Ratio ($E_b/N_0$) (dB)")
plt.ylabel("Frame Error Rate")
plt.title("Various Code Rates depending on $E_b/N_0$")

simulations = [
    ("sim_neon2.csv", "R=1/4", "tab:blue"),
    ("sim_neon3.csv", "R=1/3", "tab:red"),
    ("sim_neon4.csv", "R=1/2", "tab:orange"),
    ("sim_neon5.csv", "R=1 (Uncoded)", "tab:purple"),
]

for fichier, label, couleur in simulations:
    df = charger_csv(fichier)
    if df is not None and not df.empty:
        plt.plot(
            df["snr_bit"],
            df["fer"],
            marker="x",
            linestyle="-",
            color=couleur,
            label=label,
        )

plt.legend()
plt.tight_layout()
# 2. Sauvegarde du deuxième graphique en PNG
plt.savefig("graph_2_coderates_ebn0.png", dpi=300)
plt.show()

# ==========================================
# Graphe 3 : sim_neon2 à sim_neon5 (Différents Code Rates en fonction de snr_symbol)
# ==========================================
plt.figure(figsize=(10, 6))
plt.yscale("log")
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.xlabel("Signal-to-Noise Ratio ($E_s/N_0$) (dB)")
plt.ylabel("Frame Error Rate")
plt.title("Various Code Rates depending on $E_s/N_0$")

for fichier, label, couleur in simulations:
    df = charger_csv(fichier)
    if df is not None and not df.empty:
        plt.plot(
            df["snr_symbol"],
            df["fer"],
            marker="x",
            linestyle="-",
            color=couleur,
            label=label,
        )

plt.legend()
plt.tight_layout()
# 3. Sauvegarde du troisième graphique en PNG
plt.savefig("graph_3_coderates_esn0.png", dpi=300)
plt.show()

print("Tous les graphiques ont été affichés et sauvegardés en PNG avec succès !")
