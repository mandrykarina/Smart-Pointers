#pragma once
#include "UnqPtr.hpp"
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedListSmart
{
private:
    struct Node
    {
        T data;
        UnqPtr<Node> next;
        Node(T val) : data(val), next(nullptr) {}
    };

    UnqPtr<Node> head_;
    int length_ = 0;

public:
    LinkedListSmart() = default;

    ~LinkedListSmart()
    {
        // Разрываем цепочку вручную — безопасное поочерёдное удаление
        while (head_)
        {
            UnqPtr<Node> temp = std::move(head_->next);
            head_->next.reset(); // убираем связь
            head_.reset();       // удаляем текущий узел
            head_ = std::move(temp);
        }
    }

    void Append(T val)
    {
        UnqPtr<Node> newNode(new Node(val));
        if (!head_)
            head_ = std::move(newNode);
        else
        {
            Node *curr = head_.get();
            while (curr->next)
                curr = curr->next.get();
            curr->next = std::move(newNode);
        }
        length_++;
    }

    void Prepend(T val)
    {
        UnqPtr<Node> newNode(new Node(val));
        newNode->next = std::move(head_);
        head_ = std::move(newNode);
        length_++;
    }

    void InsertAt(T val, int index)
    {
        if (index < 0 || index > length_)
            throw std::out_of_range("Index out of range");

        if (index == 0)
        {
            Prepend(val);
            return;
        }

        Node *curr = head_.get();
        for (int i = 0; i < index - 1; ++i)
            curr = curr->next.get();

        UnqPtr<Node> newNode(new Node(val));
        newNode->next = std::move(curr->next);
        curr->next = std::move(newNode);
        length_++;
    }

    int GetLength() const { return length_; }

    void Print() const
    {
        const Node *curr = head_.get();
        std::cout << "[";
        while (curr)
        {
            std::cout << curr->data;
            if (curr->next)
                std::cout << ", ";
            curr = curr->next.get();
        }
        std::cout << "]" << std::endl;
    }
};
