#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedListRaw
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node *head_ = nullptr;
    int length_ = 0;

public:
    LinkedListRaw() = default;
    ~LinkedListRaw()
    {
        while (head_)
        {
            Node *tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
    }

    void Append(T val)
    {
        Node *newNode = new Node(val);
        if (!head_)
            head_ = newNode;
        else
        {
            Node *curr = head_;
            while (curr->next)
                curr = curr->next;
            curr->next = newNode;
        }
        length_++;
    }

    void Prepend(T val)
    {
        Node *newNode = new Node(val);
        newNode->next = head_;
        head_ = newNode;
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

        Node *curr = head_;
        for (int i = 0; i < index - 1; ++i)
            curr = curr->next;

        Node *newNode = new Node(val);
        newNode->next = curr->next;
        curr->next = newNode;
        length_++;
    }

    int GetLength() const { return length_; }
};
