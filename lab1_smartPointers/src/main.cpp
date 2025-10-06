#include <iostream>
#include "../include/UnqPtr.hpp"
#include "../include/ShrdPtr.hpp"
#include "../include/Tracker.hpp"

// Инициализация статической переменной Tracker::alive
int Tracker::alive = 0;

// -----------------------------------------------------------------------------
// Примеры тестов для UnqPtr
// -----------------------------------------------------------------------------
void test_unqptr()
{
    std::cout << "\n=== Тест UnqPtr (уникальное владение) ===" << std::endl;

    {
        UnqPtr<Tracker> p1(new Tracker());
        p1->ping();

        // Перемещение (владение переходит)
        UnqPtr<Tracker> p2 = std::move(p1);
        if (!p1)
            std::cout << "p1 теперь пуст (владение передано p2)\n";

        p2->ping();
    } // здесь p2 уничтожается → объект удаляется

    std::cout << "После выхода из блока alive = " << Tracker::alive << std::endl;
}

// -----------------------------------------------------------------------------
// Примеры тестов для ShrdPtr
// -----------------------------------------------------------------------------
void test_shrdptr()
{
    std::cout << "\n=== Тест ShrdPtr (разделяемое владение) ===" << std::endl;

    {
        ShrdPtr<Tracker> a(new Tracker());
        std::cout << "Создали a, use_count = " << a.use_count() << std::endl;

        {
            ShrdPtr<Tracker> b = a; // копируем — счётчик увеличивается
            std::cout << "Скопировали в b, use_count = " << b.use_count() << std::endl;

            {
                ShrdPtr<Tracker> c = b; // ещё одно копирование
                std::cout << "Скопировали в c, use_count = " << c.use_count() << std::endl;
                c->ping();
            } // c уничтожен → счётчик уменьшается

            std::cout << "После удаления c, use_count = " << b.use_count() << std::endl;
        } // b уничтожен → счётчик уменьшается

        std::cout << "После удаления b, use_count = " << a.use_count() << std::endl;
    } // a уничтожен → счётчик = 0, объект удаляется

    std::cout << "После выхода из блока alive = " << Tracker::alive << std::endl;
}

// -----------------------------------------------------------------------------
// Проверка работы reset() и release()
// -----------------------------------------------------------------------------
void test_reset_release()
{
    std::cout << "\n=== Тест reset() и release() ===" << std::endl;

    // --- Тест release() ---
    UnqPtr<Tracker> p(new Tracker());
    Tracker *raw = p.release(); // забрали владение
    std::cout << "После release() UnqPtr пуст? " << (p ? "нет" : "да") << std::endl;

    delete raw; // нужно вручную удалить объект
    std::cout << "После ручного delete alive = " << Tracker::alive << std::endl;

    // --- Тест reset() ---
    UnqPtr<Tracker> q(new Tracker());
    q.reset(new Tracker()); // удаляет старый, создаёт новый
    std::cout << "После reset() alive = " << Tracker::alive << std::endl;

    q.reset(); // удаляет объект, делает пустым
    std::cout << "После reset() (без аргумента) alive = " << Tracker::alive << std::endl;
}

// -----------------------------------------------------------------------------
// Главная функция
// -----------------------------------------------------------------------------
int main()
{
    std::cout << "=== Лабораторная работа №1: Умные указатели ===\n";

    test_unqptr();
    test_shrdptr();
    test_reset_release();

    std::cout << "\n=== Все тесты завершены ===" << std::endl;
    std::cout << "Оставшихся живых объектов: " << Tracker::alive << std::endl;

    return 0;
}
