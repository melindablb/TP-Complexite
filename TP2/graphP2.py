import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv("PARTIE2.csv")   


df_A = df[df["Algo"] == "A"]
df_B = df[df["Algo"] == "B"]


plt.figure(figsize=(10, 6))

plt.plot(
    df_A["N"], df_A["Temps"],
    marker="s",          
    markersize=8,
    label="Fonction MaxEtMinA"
)

plt.plot(
    df_B["N"], df_B["Temps"],
    marker="s",          
    markersize=8,
    label="Fonction MaxEtMinB"
)
plt.xticks(sorted(df["N"].unique()))
plt.ticklabel_format(style='plain', axis='x')

plt.xlabel("N (Taille du tableau)", fontweight="bold", labelpad=10) 
plt.ylabel("Temps (s)", fontweight="bold", labelpad=10) 

plt.title("Comparaison entre MaxEtMinA et MaxEtMinB",fontweight="bold")
plt.grid(True)
plt.legend()            

plt.show()
