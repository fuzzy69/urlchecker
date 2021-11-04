#include <limits>
#include <random>

#include "random.h"


using common::misc::Random;


Random::Random() : m_randomGenerator(std::random_device{}())
{
}

Random::Random(std::uint_least32_t seed) :
    m_randomGenerator(seed)
{
    
}

Random & Random::instance()
{
    static Random rand;

    return rand;
}

std::int32_t Random::get()
{
    return get(0, std::numeric_limits<std::int32_t>::max());
}

std::int32_t Random::get(std::int32_t minValue, std::int32_t maxValue)
{
    std::uniform_int_distribution<std::int32_t> distribution(minValue, maxValue);

    return distribution(m_randomGenerator);
}

