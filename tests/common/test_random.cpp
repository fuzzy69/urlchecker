#include <iostream>
#include <string>

#include "catch.hpp"

#include "common/random.h"


using common::misc::Random;

TEST_CASE( "Test Random class", "[random]" )
{
    int x;
    for (int i = 0; i < 100; ++i)
    {
        x = Random::instance().get(0, 10);
        REQUIRE((x >= 0 && x <= 10));
    }
}
