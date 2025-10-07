#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include "../include/LinkedList.hpp"
#include "../include/LinkedListRaw.hpp"
#include "../include/Tracker.hpp"

void testPerformance()
{
    std::ofstream csv("../report/results.csv");
    csv << "N,Smart_ms,Raw_ms,Smart_alive,Raw_alive\n";

    std::vector<int> sizes = {1000, 10000, 100000};

    for (int N : sizes)
    {
        // --- Smart (UnqPtr) ---
        auto start = std::chrono::high_resolution_clock::now();
        {
            LinkedListSmart<Tracker> list;
            for (int i = 0; i < N; ++i)
                list.Append(Tracker());
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto smart_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        int smart_alive = Tracker::alive;

        // --- Raw (обычные указатели) ---
        start = std::chrono::high_resolution_clock::now();
        {
            LinkedListRaw<Tracker> list;
            for (int i = 0; i < N; ++i)
                list.Append(Tracker());
        }
        end = std::chrono::high_resolution_clock::now();
        auto raw_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        int raw_alive = Tracker::alive;

        csv << N << "," << smart_ms << "," << raw_ms << "," << smart_alive << "," << raw_alive << "\n";

        std::cout << "N=" << N
                  << " | Smart=" << smart_ms << "ms"
                  << " | Raw=" << raw_ms << "ms"
                  << " | Alive(Smart/Raw)=" << smart_alive << "/" << raw_alive << std::endl;
    }

    csv.close();
}

int main()
{
    std::cout << "\n===== НАГРУЗОЧНЫЕ ТЕСТЫ LinkedList =====\n";
    testPerformance();
    std::cout << "\nРезультаты сохранены в report/results.csv\n";
    return 0;
}
