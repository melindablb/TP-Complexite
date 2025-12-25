import pandas as pd
import matplotlib.pyplot as plt

# Mettre le fichier CSV ici
csv_file = "Algo1.csv" 

# Lire le fichier CSV
data = pd.read_csv(csv_file)

# Colonnes à adapter selon votre CSV
col_algo = "num_algo"   
col_n = "n"              
col_time = "temps_moyen"   

# Filtrer uniquement l'algo 1
subset = data[data[col_algo] == 2]

# Tracer
plt.figure()
plt.plot(
    subset[col_n],
    subset[col_time],
    marker='o',
    label="Algo 2 : Tri à bulles non optimisé"
)

plt.xlabel("Taille du tableau (n)")  
plt.ylabel("Temps d'exécution moyen (s)") 
plt.title("Évolution du temps d'exécution en fonction de n")
plt.ticklabel_format(style='plain', axis='x')


plt.legend()
plt.grid(True)
plt.show()
