import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Пути к файлам
csv_path = Path(__file__).resolve().parent / "results.csv"
out_time = Path(__file__).resolve().parent / "plot_time.png"
out_memory = Path(__file__).resolve().parent / "plot_memory.png"

# Загружаем CSV
data = pd.read_csv(csv_path)

# ==============================
# 📊 1. ГРАФИК ВРЕМЕНИ
# ==============================
plt.figure(figsize=(8, 5))
plt.plot(data["N"], data["Smart_ms"], "o-", label="LinkedList (с UnqPtr)", linewidth=2)
plt.plot(data["N"], data["Raw_ms"], "s--", label="LinkedList (обычные указатели)", linewidth=2)

plt.title("Сравнение производительности LinkedList", fontsize=13)
plt.xlabel("Количество элементов N")
plt.ylabel("Время выполнения, мс")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig(out_time, dpi=150)
print(f"✅ График времени сохранён: {out_time}")

# ==============================
# 📈 2. ГРАФИК ПАМЯТИ
# ==============================
plt.figure(figsize=(8, 5))
plt.plot(data["N"], data["Smart_alive"], "o-", label="Живые объекты (Smart)", linewidth=2, color="green")
plt.plot(data["N"], data["Raw_alive"], "s--", label="Живые объекты (Raw)", linewidth=2, color="red")

plt.title("Сравнение использования памяти (живые объекты Tracker)", fontsize=13)
plt.xlabel("Количество элементов N")
plt.ylabel("Количество неосвобождённых объектов")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig(out_memory, dpi=150)
print(f"✅ График памяти сохранён: {out_memory}")

print("\n📊 Оба графика успешно построены!")
