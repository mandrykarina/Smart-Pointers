#include <iostream>
#include <cassert> // для assert()
#include "../include/UnqPtr.hpp"
#include "../include/ShrdPtr.hpp"
#include "../include/Tracker.hpp"

// Инициализация статической переменной Tracker::alive
int Tracker::alive = 0;

// -----------------------------------------------------------------------------
// Проверка работы UnqPtr
// -----------------------------------------------------------------------------
void test_unqptr_basic()
{
    std::cout << "\n[TEST] UnqPtr — базовые проверки" << std::endl;

    {
        UnqPtr<Tracker> p1(new Tracker());
        assert(p1); // должен быть не пустым

        // Проверяем работу разыменования и ->
        p1->ping();
        (*p1).ping();

        // Проверяем перемещение
        UnqPtr<Tracker> p2 = std::move(p1);
        assert(!p1); // после move должен стать пустым
        assert(p2);  // p2 владеет объектом

        p2->ping();

        // Проверяем reset()
        p2.reset(new Tracker());
        assert(p2); // всё ещё владеет
    }

    // после выхода из блока всё должно быть уничтожено
    assert(Tracker::alive == 0);
    std::cout << "✅ UnqPtr работает корректно, утечек нет." << std::endl;
}

// -----------------------------------------------------------------------------
// Проверка работы ShrdPtr
// -----------------------------------------------------------------------------
void test_shrdptr_basic()
{
    std::cout << "\n[TEST] ShrdPtr — базовые проверки" << std::endl;

    {
        ShrdPtr<Tracker> a(new Tracker());
        assert(a);
        assert(a.use_count() == 1);

        {
            ShrdPtr<Tracker> b = a; // копия
            assert(a.use_count() == 2);
            assert(b.use_count() == 2);
            b->ping();

            {
                ShrdPtr<Tracker> c = b; // ещё одна копия
                assert(a.use_count() == 3);
                assert(c.use_count() == 3);
                c->ping();
            } // c уничтожен

            assert(a.use_count() == 2);
            assert(b.use_count() == 2);
        } // b уничтожен

        assert(a.use_count() == 1);
    }

    // после выхода из блока должен быть уничтожен последний объект
    assert(Tracker::alive == 0);
    std::cout << "✅ ShrdPtr работает корректно, счётчик ссылок верный." << std::endl;
}

// -----------------------------------------------------------------------------
// Проверка reset() и release() для UnqPtr
// -----------------------------------------------------------------------------
void test_unqptr_reset_release()
{
    std::cout << "\n[TEST] UnqPtr — reset() и release()" << std::endl;

    {
        UnqPtr<Tracker> p(new Tracker());
        assert(Tracker::alive == 1);

        Tracker *raw = p.release();
        assert(!p); // p теперь пуст
        assert(raw != nullptr);
        delete raw; // вручную удаляем
        assert(Tracker::alive == 0);

        // Проверяем reset()
        p.reset(new Tracker());
        assert(Tracker::alive == 1);

        p.reset(); // удалить объект
        assert(!p);
        assert(Tracker::alive == 0);
    }

    std::cout << "✅ UnqPtr.reset() и release() работают корректно." << std::endl;
}

// -----------------------------------------------------------------------------
// Запуск всех базовых тестов
// -----------------------------------------------------------------------------
void run_basic_tests()
{
    std::cout << "\n===============================" << std::endl;
    std::cout << "   БАЗОВЫЕ ТЕСТЫ SMART POINTERS" << std::endl;
    std::cout << "===============================" << std::endl;

    test_unqptr_basic();
    test_shrdptr_basic();
    test_unqptr_reset_release();

    std::cout << "\n✅ Все базовые тесты успешно пройдены!" << std::endl;
}
