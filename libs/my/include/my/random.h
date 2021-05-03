#pragma once

#include <random>


namespace my
{
namespace misc
{
class Random final
{
public:
    Random(std::uint_least32_t seed);
    Random(const Random &other) = delete;
    void operator=(const Random &other) = delete;

    static Random& instance();

    std::int32_t get();
    std::int32_t get(std::int32_t minValue, std::int32_t maxValue);

private:
    Random();

    std::mt19937 m_randomGenerator;
};
}
}
