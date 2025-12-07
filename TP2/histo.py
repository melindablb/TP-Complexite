import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


df = pd.read_csv("PARTIE2.csv")


df_A = df[df["Algo"] == "A"]
df_B = df[df["Algo"] == "B"]


x = np.arange(len(df_A["N"])) 
width = 0.35 

plt.figure(figsize=(10, 6))

plt.bar(x - width/2, df_A["Compteur"], width, label="Fonction MaxEtMinA")
plt.bar(x + width/2, df_B["Compteur"], width, label="Fonction MaxEtMinB")


plt.xticks(x, df_A["N"], rotation=0)  

plt.ticklabel_format(style='plain', axis='y')

plt.xlabel("N (Taille du tableau)", fontweight="bold", labelpad=10)
plt.ylabel("Nombre de comparaisons", fontweight="bold", labelpad=10)
plt.title("Comparaison du nombre de comparaisons", fontweight="bold")
plt.legend()
plt.grid(axis="y")

plt.show()
