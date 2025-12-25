import pandas as pd
import matplotlib.pyplot as plt

# Fichier CSV
csv_file = "Algo1.csv" 

# Lire le CSV
data = pd.read_csv(csv_file)

# Colonnes à adapter selon votre CSV
col_algo = "num_algo"   
col_n = "n"              
col_time = "temps_moyen"   

# Filtrer uniquement les algos qui nous intéressent
subset = data[data[col_algo].isin([1, 2])]  # ici on prend 1 et 2, par exemple

plt.figure()

for algo in subset[col_algo].unique():
    algo_subset = subset[subset[col_algo] == algo]
    
    # Choix du label selon la valeur de num_algo
    if algo == 1:
        label = "Tri à bulles"
    else:
        label = "Tri à bulles optimisé"
    
    plt.plot(
        algo_subset[col_n],
        algo_subset[col_time],
        marker='o',
        label=label
    )

plt.xlabel("Taille du tableau (n)")  
plt.ylabel("Temps d'exécution moyen (s)") 
plt.title("Évolution du temps d'exécution en fonction de n")
plt.ticklabel_format(style='plain', axis='x')
plt.legend()
plt.grid(True)
plt.show()
