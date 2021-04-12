#include <limits>
#include <random>

#include "rand.h"


Rand::Rand() : m_randomGenerator(std::random_device{}())
{
}

Rand::Rand(std::uint_least32_t seed) :
    m_randomGenerator(seed)
{
    
}

Rand & Rand::instance()
{
    static Rand rand;

    return rand;
}

std::int32_t Rand::get()
{
    return get(0, std::numeric_limits<std::int32_t>::max());
}

std::int32_t Rand::get(std::int32_t minValue, std::int32_t maxValue)
{
    std::uniform_int_distribution<std::int32_t> distribution(minValue, maxValue);

    return distribution(m_randomGenerator);
}
