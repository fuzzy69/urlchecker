#pragma once

#include <random>


class Random
{
public:
    Random();

    static int get(int low, int high);

private:
    std::random_device m_randomDevice;
    std::mt19937 m_engine;
};
