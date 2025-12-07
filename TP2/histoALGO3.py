import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Charger le CSV
df = pd.read_csv("dicho.csv")   # remplace par ton nom de fichier

# Extraire les colonnes utiles
N = df["N"]
worst = df["WorstCase"]
best = df["BestCase"]

# Largeur des barres
width = 0.35

# Position de base pour chaque N
x = np.arange(len(N))

# Création du plot
plt.figure(figsize=(10, 6))

plt.bar(x - width/2, worst, width, label="Pire Cas")
plt.bar(x + width/2, best, width, label="Meilleur Cas")

# Nom des axes
plt.xlabel("N (Taille du tableau)",fontweight="bold")
plt.ylabel("Temps (secondes)",fontweight="bold")
plt.title("Pire vs Meilleur cas de la recherche dichotomique en fonction de N",fontweight="bold")
plt.ticklabel_format(style='plain', axis='y')

# Mettre les valeurs de N sur l'axe X
plt.xticks(x, N)

plt.legend()
plt.show()
