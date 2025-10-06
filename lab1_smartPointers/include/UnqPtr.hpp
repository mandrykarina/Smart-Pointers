#pragma once
#include <utility> // std::move, std::exchange

// -----------------------------------------------------------------------------
// Класс UnqPtr<T>
// Умный указатель, который владеет объектом эксклюзивно.
// Нельзя копировать, можно только перемещать.
// -----------------------------------------------------------------------------
template <typename T>
class UnqPtr
{
private:
    T *ptr_; // указатель на управляемый объект

public:
    // --- 1. Конструктор по умолчанию ---
    UnqPtr() noexcept : ptr_(nullptr)
    {
        // создаём "пустой" умный указатель, ни на что не указывает
    }

    // --- 2. Конструктор от обычного указателя ---
    explicit UnqPtr(T *p) noexcept : ptr_(p)
    {
        // теперь UnqPtr "владеет" этим объектом
    }

    // --- 3. Деструктор ---
    ~UnqPtr()
    {
        delete ptr_; // освобождает память, если объект есть
    }

    // --- 4. Копирование запрещено ---
    UnqPtr(const UnqPtr &) = delete;
    UnqPtr &operator=(const UnqPtr &) = delete;

    // --- 5. Перемещающий конструктор ---
    UnqPtr(UnqPtr &&other) noexcept : ptr_(std::exchange(other.ptr_, nullptr))
    {
        // забираем объект у другого UnqPtr, делая его пустым
    }

    // --- 6. Перемещающее присваивание ---
    UnqPtr &operator=(UnqPtr &&other) noexcept
    {
        if (this != &other)
        {
            delete ptr_;                               // удаляем старый объект
            ptr_ = std::exchange(other.ptr_, nullptr); // забираем новый
        }
        return *this;
    }

    // --- 7. Получить "сырой" указатель ---
    T *get() const noexcept
    {
        return ptr_;
    }

    // --- 8. Отпустить объект (без удаления) ---
    T *release() noexcept
    {
        // отдаём объект наружу, UnqPtr больше не владеет им
        return std::exchange(ptr_, nullptr);
    }

    // --- 9. Заменить управляемый объект ---
    void reset(T *p = nullptr) noexcept
    {
        if (ptr_ != p)
        {
            delete ptr_; // удаляем старый объект
            ptr_ = p;    // начинаем владеть новым
        }
    }

    // --- 10. Обменяться объектами ---
    void swap(UnqPtr &other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }

    // --- 11. Разыменование ---
    T &operator*() const
    {
        return *ptr_; // возвращаем сам объект
    }

    // --- 12. Доступ к членам объекта ---
    T *operator->() const
    {
        return ptr_;
    }

    // --- 13. Проверка, есть ли объект ---
    explicit operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }
};
