#pragma once
#include <iostream>

class Tracker
{
public:
    static int alive;
    int id;

    Tracker()
    {
        id = ++alive;
        // std::cout << "[Tracker " << id << "] создан. Alive = " << alive << std::endl;
    }

    ~Tracker()
    {
        // std::cout << "[Tracker " << id << "] уничтожен. Alive = " << (alive - 1) << std::endl;
        --alive;
    }

    void ping() const
    {
        // std::cout << "[Tracker " << id << "] работает." << std::endl;
    }
};
