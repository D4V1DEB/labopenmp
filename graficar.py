import os
import pandas as pd
import matplotlib.pyplot as plt

# crear carpeta de salida
os.makedirs("graficos", exist_ok=True)

# estilo general limpio para reportes
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
plt.rcParams['font.size'] = 10
plt.rcParams['figure.titlesize'] = 12

# graficas de odd-even transposition sort
if os.path.exists("tiempos_oddeven.csv"):
    df_oe = pd.read_csv("tiempos_oddeven.csv")

    # speedup vs hilos para arreglo aleatorio
    plt.figure(figsize=(8, 5))
    df_aleatorio = df_oe[df_oe['tipo_arreglo'] == 'aleatorio']
    tamanos = df_aleatorio['tamano'].unique()
    
    for n in sorted(tamanos):
        sub = df_aleatorio[df_aleatorio['tamano'] == n].sort_values('hilos')
        plt.plot(sub['hilos'], sub['speedup'], marker='o', label=f'N = {n}')
    
    # linea ideal de speedup
    hilos_unicos = sorted(df_aleatorio['hilos'].unique())
    plt.plot(hilos_unicos, hilos_unicos, 'k--', alpha=0.5, label='Ideal (Lineal)')
    plt.title("Odd-Even Sort: Speedup vs Número de Hilos (Arreglo Aleatorio)")
    plt.xlabel("Cantidad de Hilos")
    plt.ylabel("Speedup (T_seq / T_par)")
    plt.xticks(hilos_unicos)
    plt.legend()
    plt.tight_layout()
    plt.savefig("graficos/oddeven_speedup_aleatorio.png", dpi=300)
    plt.close()

    # eficiencia vs hilos para arreglo aleatorio
    plt.figure(figsize=(8, 5))
    for n in sorted(tamanos):
        sub = df_aleatorio[df_aleatorio['tamano'] == n].sort_values('hilos')
        plt.plot(sub['hilos'], sub['eficiencia'], marker='s', label=f'N = {n}')
    
    plt.axhline(1.0, color='black', linestyle='--', alpha=0.5, label='Ideal (100%)')
    plt.title("Odd-Even Sort: Eficiencia vs Número de Hilos (Arreglo Aleatorio)")
    plt.xlabel("Cantidad de Hilos")
    plt.ylabel("Eficiencia (Speedup / Hilos)")
    plt.xticks(hilos_unicos)
    plt.ylim(0, 1.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig("graficos/oddeven_eficiencia_aleatorio.png", dpi=300)
    plt.close()

    # tiempo paralelo vs tamano por tipo de arreglo con 4 hilos
    plt.figure(figsize=(8, 5))
    df_4h = df_oe[df_oe['hilos'] == 4]
    for tipo in df_oe['tipo_arreglo'].unique():
        sub = df_4h[df_4h['tipo_arreglo'] == tipo].sort_values('tamano')
        plt.plot(sub['tamano'], sub['tiempo_paralelo'], marker='^', label=f'Tipo: {tipo}')
    
    plt.title("Odd-Even Sort: Tiempo Paralelo vs Tamaño N (4 Hilos)")
    plt.xlabel("Tamaño del Arreglo (N)")
    plt.ylabel("Tiempo de Ejecución (s)")
    plt.legend()
    plt.tight_layout()
    plt.savefig("graficos/oddeven_tiempo_por_tipo.png", dpi=300)
    plt.close()

    # speedup en n = 40000 para los 4 tipos de arreglos
    plt.figure(figsize=(8, 5))
    df_40k = df_oe[df_oe['tamano'] == 40000]
    for tipo in df_40k['tipo_arreglo'].unique():
        sub = df_40k[df_40k['tipo_arreglo'] == tipo].sort_values('hilos')
        plt.plot(sub['hilos'], sub['speedup'], marker='D', label=f'Tipo: {tipo}')
    
    plt.axhline(1.0, color='red', linestyle=':', label='Límite Secuencial (S=1)')
    plt.title("Odd-Even Sort: Speedup vs Hilos (N = 40,000 en los 4 tipos)")
    plt.xlabel("Cantidad de Hilos")
    plt.ylabel("Speedup")
    plt.xticks(hilos_unicos)
    plt.legend()
    plt.tight_layout()
    plt.savefig("graficos/oddeven_speedup_40k.png", dpi=300)
    plt.close()

# graficas de lista enlazada comparacion
if os.path.exists("tiempos_bloqueo_global.csv") and os.path.exists("tiempos_grano_fino.csv"):
    df_global = pd.read_csv("tiempos_bloqueo_global.csv").sort_values('operaciones')
    df_fino = pd.read_csv("tiempos_grano_fino.csv").sort_values('operaciones')

    fig, axes = plt.subplots(1, 2, figsize=(13, 5))

    # tiempo de insercion
    axes[0].plot(df_global['operaciones'], df_global['tiempo_insercion'], marker='o', color='tab:blue', label='Bloqueo Global')
    axes[0].plot(df_fino['operaciones'], df_fino['tiempo_insercion'], marker='s', color='tab:orange', label='Grano Fino (Hand-over-Hand)')
    axes[0].set_title("Lista Enlazada: Tiempo de Inserción vs Operaciones")
    axes[0].set_xlabel("Número de Operaciones (Nodos)")
    axes[0].set_ylabel("Tiempo (s)")
    axes[0].legend()

    # tiempo de busqueda en escala logaritmica
    axes[1].plot(df_global['operaciones'], df_global['tiempo_busqueda'], marker='o', color='tab:blue', label='Bloqueo Global (sin locks en lectura)')
    axes[1].plot(df_fino['operaciones'], df_fino['tiempo_busqueda'], marker='s', color='tab:orange', label='Grano Fino (lock/unlock por nodo)')
    axes[1].set_yscale('log')
    axes[1].set_title("Lista Enlazada: Tiempo de Búsqueda vs Operaciones (Escala Log)")
    axes[1].set_xlabel("Número de Operaciones (Nodos)")
    axes[1].set_ylabel("Tiempo (s) - Log Scale")
    axes[1].legend()

    plt.tight_layout()
    plt.savefig("graficos/listaenlazada_comparacion.png", dpi=300)
    plt.close()

print("Graficas generadas exitosamente en la carpeta 'graficos/'.")
