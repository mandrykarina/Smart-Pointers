#include <iostream>
#include "../include/UnqPtr.hpp"
#include "../include/Tracker.hpp"

void test_basic_creation()
{
    std::cout << "\n=== Тест 1: Базовое создание и уничтожение ===" << std::endl;
    {
        UnqPtr<Tracker> ptr(new Tracker());
        std::cout << "🧠 Указатель создан. ptr -> Tracker #" << ptr->id << std::endl;
    }
    std::cout << "🧹 Область видимости закончилась — объект должен быть удалён автоматически." << std::endl;
}

void test_move_semantics()
{
    std::cout << "\n=== Тест 2: Перемещение владения (move semantics) ===" << std::endl;
    {
        UnqPtr<Tracker> a(new Tracker());
        std::cout << "✅ Создан ptr a -> Tracker #" << a->id << std::endl;

        UnqPtr<Tracker> b = std::move(a);
        std::cout << "➡️ Владение перемещено из a в b." << std::endl;
        std::cout << "   a теперь " << (a ? "не пуст" : "пуст") << std::endl;
        std::cout << "   b теперь указывает на Tracker #" << b->id << std::endl;
    }
    std::cout << "🧹 При выходе из блока b уничтожается, объект освобождён." << std::endl;
}

void test_reset_release_swap()
{
    std::cout << "\n=== Тест 3: reset(), release(), swap() ===" << std::endl;

    UnqPtr<Tracker> p1(new Tracker());
    UnqPtr<Tracker> p2(new Tracker());

    std::cout << "\n--- Исходное состояние ---" << std::endl;
    std::cout << "p1 -> Tracker #" << p1->id << ", p2 -> Tracker #" << p2->id << std::endl;

    std::cout << "\n🌀 Обмен (swap) p1 и p2..." << std::endl;
    p1.swap(p2);
    std::cout << "Теперь p1 -> Tracker #" << p1->id << ", p2 -> Tracker #" << p2->id << std::endl;

    std::cout << "\n🔁 reset(): переустанавливаем p1..." << std::endl;
    p1.reset(new Tracker());
    std::cout << "p1 теперь указывает на нового Tracker #" << p1->id << std::endl;

    std::cout << "\n📤 release(): отпускаем владение из p1..." << std::endl;
    Tracker *raw = p1.release();
    std::cout << "Теперь p1 " << (p1 ? "не пуст" : "пуст") << std::endl;
    std::cout << "Но объект #" << raw->id << " остался жив (ручное удаление)!" << std::endl;
    delete raw;

    std::cout << "🧹 По завершении функции оставшиеся указатели очистятся автоматически." << std::endl;
}

int main()
{
    std::cout << "==============================" << std::endl;
    std::cout << "   ТЕСТЫ УМНОГО УКАЗАТЕЛЯ UnqPtr" << std::endl;
    std::cout << "==============================" << std::endl;

    test_basic_creation();
    test_move_semantics();
    test_reset_release_swap();

    std::cout << "\n✅ Все функциональные тесты завершены. Активных объектов: " << Tracker::alive << std::endl;
    return 0;
}
