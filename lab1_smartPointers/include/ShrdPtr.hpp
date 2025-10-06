#pragma once
#include <cstddef>     // size_t
#include <utility>     // std::exchange, std::swap
#include <type_traits> // std::is_convertible

// -----------------------------------------------------------------------------
// Класс ShrdPtr<T>
// Умный указатель с разделяемым владением (аналог std::shared_ptr).
// Хранит:
//   - сам объект (ptr_)
//   - счётчик ссылок (refcount_), общий для всех копий
// -----------------------------------------------------------------------------
template <typename T>
class ShrdPtr
{
private:
    T *ptr_;           // указатель на сам объект
    size_t *refcount_; // указатель на счётчик ссылок (в куче)

    // Вспомогательная функция для уменьшения счётчика и удаления объекта
    void release_ref() noexcept
    {
        if (refcount_)
        {
            if (--(*refcount_) == 0)
            {                     // если это была последняя ссылка
                delete ptr_;      // удаляем сам объект
                delete refcount_; // удаляем счётчик
            }
            refcount_ = nullptr;
            ptr_ = nullptr;
        }
    }

public:
    // --- 1. Конструктор по умолчанию ---
    ShrdPtr() noexcept : ptr_(nullptr), refcount_(nullptr)
    {
        // пустой указатель — ни на что не ссылается
    }

    // --- 2. Конструктор от "сырого" указателя ---
    explicit ShrdPtr(T *p) noexcept : ptr_(p), refcount_(nullptr)
    {
        if (p)
        {
            refcount_ = new size_t(1); // создаём счётчик = 1
        }
    }

    // --- 3. Копирующий конструктор ---
    ShrdPtr(const ShrdPtr &other) noexcept : ptr_(other.ptr_), refcount_(other.refcount_)
    {
        if (refcount_)
        {
            ++(*refcount_); // увеличиваем счётчик ссылок
        }
    }

    // --- 4. Перемещающий конструктор ---
    ShrdPtr(ShrdPtr &&other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)), refcount_(std::exchange(other.refcount_, nullptr))
    {
        // забрали владение и обнулили другой указатель
    }

    // --- 5. Деструктор ---
    ~ShrdPtr()
    {
        release_ref(); // уменьшаем счётчик, удаляем если последний
    }

    // --- 6. Копирующее присваивание ---
    ShrdPtr &operator=(const ShrdPtr &other) noexcept
    {
        if (this != &other)
        {
            release_ref(); // уменьшаем свой счётчик (и удаляем, если нужно)
            ptr_ = other.ptr_;
            refcount_ = other.refcount_;
            if (refcount_)
            {
                ++(*refcount_); // увеличиваем счётчик новой ссылки
            }
        }
        return *this;
    }

    // --- 7. Перемещающее присваивание ---
    ShrdPtr &operator=(ShrdPtr &&other) noexcept
    {
        if (this != &other)
        {
            release_ref(); // освобождаем текущие ресурсы
            ptr_ = std::exchange(other.ptr_, nullptr);
            refcount_ = std::exchange(other.refcount_, nullptr);
        }
        return *this;
    }

    // --- 8. Счётчик ссылок ---
    size_t use_count() const noexcept
    {
        return refcount_ ? *refcount_ : 0;
    }

    // --- 9. Получить "сырой" указатель ---
    T *get() const noexcept
    {
        return ptr_;
    }

    // --- 10. Сбросить управление (удалить старый, начать новый) ---
    void reset(T *p = nullptr)
    {
        release_ref(); // уменьшаем старую ссылку
        if (p)
        {
            ptr_ = p;
            refcount_ = new size_t(1);
        }
        else
        {
            ptr_ = nullptr;
            refcount_ = nullptr;
        }
    }

    // --- 11. Обменяться с другим ShrdPtr ---
    void swap(ShrdPtr &other) noexcept
    {
        std::swap(ptr_, other.ptr_);
        std::swap(refcount_, other.refcount_);
    }

    // --- 12. Разыменование и доступ ---
    T &operator*() const
    {
        return *ptr_;
    }

    T *operator->() const
    {
        return ptr_;
    }

    // --- 13. Проверка на наличие объекта ---
    explicit operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }
};
