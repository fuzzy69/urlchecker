#include <iostream>
#include <string>

#include "catch.hpp"

#include "my/random.h"


using my::misc::Random;

TEST_CASE( "Test Rand class", "[rand]" )
{
    int x;
    for (int i = 0; i < 100; ++i)
    {
        x = Random::instance().get(0, 10);
        REQUIRE((x >= 0 && x <= 10));
    }
}
