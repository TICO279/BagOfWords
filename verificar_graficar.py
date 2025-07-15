import pandas as pd
import matplotlib.pyplot as plt

# === Load CSVs ===
serial_df = pd.read_csv("matriz_serial.csv", index_col=0)
paralelo_df = pd.read_csv("matriz_paralela.csv", index_col=0)

# === 1. Verificar si son iguales ===
if serial_df.equals(paralelo_df):
    print("✅ Las matrices son exactamente iguales.")
else:
    diff = (serial_df != paralelo_df).sum().sum()
    print(f"⚠️ Las matrices NO son iguales. Diferencias encontradas: {diff}")

# === 2. Ingresar tiempos de ejecución manualmente si no se automatiza ===
tiempo_serial = float(input("⏱️ Tiempo de ejecución (serial) en segundos: "))
tiempo_paralelo = float(input("⏱️ Tiempo de ejecución (paralelo) en segundos: "))

# === 3. Calcular Speed-Up ===
speedup = tiempo_serial / tiempo_paralelo
print(f"🚀 Speed-Up: {speedup:.2f}x")

# === 4. Graficar ===
plt.figure(figsize=(6, 4))
plt.bar(["Serial", "Paralelo"], [tiempo_serial, tiempo_paralelo], color=["gray", "green"])
plt.title("Comparación de Tiempos de Ejecución")
plt.ylabel("Tiempo (segundos)")
plt.text(1, tiempo_paralelo + 0.02, f"Speed-Up: {speedup:.2f}x", ha='center', fontsize=10, color='blue')
plt.tight_layout()
plt.savefig("grafica_speedup.png")
plt.show()
