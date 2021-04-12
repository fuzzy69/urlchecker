#pragma once

#include <random>


class Rand final
{
public:
    Rand(std::uint_least32_t seed);
    Rand(const Rand &other) = delete;
    void operator=(const Rand &other) = delete;

    static Rand& instance();

    std::int32_t get();
    std::int32_t get(std::int32_t minValue, std::int32_t maxValue);

private:
    Rand();

    std::mt19937 m_randomGenerator;
};
