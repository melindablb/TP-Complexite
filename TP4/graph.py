import pandas as pd
import matplotlib.pyplot as plt

# Lire le CSV
df = pd.read_csv("algo3.csv")

# Tracer
plt.plot(df["n"], df["temps"], marker='o')
plt.xlabel("Taille n du tableau",fontweight='bold')
plt.ylabel("Temps (s)",fontweight='bold')
plt.title("Temps d'execution en fonction de la taille n du tableau",fontweight='bold')
plt.grid(True)
plt.ticklabel_format(style='plain', axis='x')
plt.show()
