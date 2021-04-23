#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/utils/rand.h"


TEST_CASE( "Test Rand class", "[rand]" )
{
    int x;
    for (int i = 0; i < 100; ++i)
    {
        x = Rand::instance().get(0, 10);
        REQUIRE((x >= 0 && x <= 10));
    }
}
