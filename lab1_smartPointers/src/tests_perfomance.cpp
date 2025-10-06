#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "../include/UnqPtr.hpp"
#include "../include/ShrdPtr.hpp"
#include "../include/Tracker.hpp"

using namespace std;
using namespace std::chrono;

int Tracker::alive = 0;

// ----------------------------------------------------------
// Функция измерения времени выполнения
// ----------------------------------------------------------
template <typename Func>
double measure_time(Func f)
{
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

// ----------------------------------------------------------
// Тестовые функции
// ----------------------------------------------------------
void test_raw(size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        Tracker *t = new Tracker();
        delete t;
    }
}

void test_unqptr(size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        UnqPtr<Tracker> t(new Tracker());
    }
}

void test_shrdptr(size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        ShrdPtr<Tracker> t(new Tracker());
    }
}

// ----------------------------------------------------------
// Красивый вывод результатов
// ----------------------------------------------------------
void print_table_header()
{
    cout << "\n============================================\n";
    cout << "📊  РЕЗУЛЬТАТЫ ТЕСТА ПРОИЗВОДИТЕЛЬНОСТИ\n";
    cout << "============================================\n";
    cout << left << setw(12) << "N объектов"
         << setw(18) << "Raw (мс)"
         << setw(18) << "UnqPtr (мс)"
         << setw(18) << "ShrdPtr (мс)" << endl;
    cout << string(65, '-') << endl;
}

void print_table_row(size_t N, double t_raw, double t_unq, double t_shr)
{
    cout << left << setw(12) << N
         << setw(18) << t_raw
         << setw(18) << t_unq
         << setw(18) << t_shr << endl;
}

void print_ascii_chart(const vector<size_t> &Ns,
                       const vector<double> &t_raw,
                       const vector<double> &t_unq,
                       const vector<double> &t_shr)
{
    cout << "\n============================================\n";
    cout << "📈  ASCII-график (чем длиннее полоса — тем дольше)\n";
    cout << "============================================\n\n";

    // ищем максимальное время, чтобы масштабировать длину графика
    double max_t = max({*max_element(t_raw.begin(), t_raw.end()),
                        *max_element(t_unq.begin(), t_unq.end()),
                        *max_element(t_shr.begin(), t_shr.end())});

    int width = 50;
    for (size_t i = 0; i < Ns.size(); ++i)
    {
        cout << "N = " << Ns[i] << endl;

        int len_raw = static_cast<int>(t_raw[i] / max_t * width);
        int len_unq = static_cast<int>(t_unq[i] / max_t * width);
        int len_shr = static_cast<int>(t_shr[i] / max_t * width);

        cout << "RawPtr   | " << string(len_raw, '#') << endl;
        cout << "UnqPtr   | " << string(len_unq, '#') << endl;
        cout << "ShrdPtr  | " << string(len_shr, '#') << endl;
        cout << endl;
    }
}

void plot_results()
{
    std::ofstream py("plot_script.py");

    py << R"(
import matplotlib.pyplot as plt
import pandas as pd
from pathlib import Path

# корректный путь к CSV (на уровень выше, в папку report)
csv_path = Path(__file__).resolve().parent.parent / 'report' / 'results.csv'
out_path = Path(__file__).resolve().parent.parent / 'report' / 'plots.png'

print(f'Читаем файл: {csv_path}')

data = pd.read_csv(csv_path)

plt.figure(figsize=(8,5))
plt.plot(data['N'], data['Raw_ms'], 'o-', label='Raw Pointer', color='blue')
plt.plot(data['N'], data['UnqPtr_ms'], 'o-', label='UnqPtr', color='green')
plt.plot(data['N'], data['ShrdPtr_ms'], 'o-', label='ShrdPtr', color='red')

plt.xlabel('Количество объектов N')
plt.ylabel('Время, мс')
plt.title('Сравнение производительности указателей')
plt.legend()
plt.grid(True)
plt.savefig(out_path, dpi=150)
print(f'✅ График сохранён: {out_path}')
)";

    py.close();
    std::system("python plot_script.py");
    std::remove("plot_script.py");
}

// ----------------------------------------------------------
// Главная функция
// ----------------------------------------------------------
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<size_t> Ns = {1000, 10000, 100000, 1000000}; // можно увеличить
    vector<double> times_raw, times_unq, times_shr;

    // CSV для отчёта
    ofstream csv("../report/results.csv");
    csv << "N,Raw_ms,UnqPtr_ms,ShrdPtr_ms\n";

    print_table_header();

    for (size_t N : Ns)
    {
        Tracker::alive = 0;
        double t_raw = measure_time([&]()
                                    { test_raw(N); });

        Tracker::alive = 0;
        double t_unq = measure_time([&]()
                                    { test_unqptr(N); });

        Tracker::alive = 0;
        double t_shr = measure_time([&]()
                                    { test_shrdptr(N); });

        print_table_row(N, t_raw, t_unq, t_shr);

        csv << N << "," << t_raw << "," << t_unq << "," << t_shr << "\n";

        times_raw.push_back(t_raw);
        times_unq.push_back(t_unq);
        times_shr.push_back(t_shr);
    }

    csv.close();
    print_ascii_chart(Ns, times_raw, times_unq, times_shr);

    cout << "\nВсе данные сохранены в report/results.csv ✅" << endl;
    cout << "Можно построить графики в Excel, LibreOffice или Google Sheets." << endl;
    plot_results();
}
