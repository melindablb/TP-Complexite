import pandas as pd
import matplotlib.pyplot as plt

# Charger le fichier CSV
df = pd.read_csv("recherche_all.csv")

# Identifier automatiquement tous les algos présents dans le fichier
algos = df["Algo"].unique()

plt.figure(figsize=(12, 6))

# Pour chaque algo, on trace sa courbe WorstCase
for algo in algos:
    df_algo = df[df["Algo"] == algo]

    plt.plot(
        df_algo["N"],
        df_algo["WorstCase"],
        marker="o",
        markersize=7,
        label=f"Algo {algo} (WorstCase)"
    )

# Mise en forme du graphique
plt.xticks(sorted(df["N"].unique()))
plt.ticklabel_format(style='plain', axis='x')

plt.xlabel("N (Taille du tableau)", fontweight="bold", labelpad=10)
plt.ylabel("Worst Case (s)", fontweight="bold", labelpad=10)

plt.title("Comparaison des WorstCase pour les 3 Algorithmes", fontweight="bold")

plt.grid(True)
plt.legend()

plt.show()
