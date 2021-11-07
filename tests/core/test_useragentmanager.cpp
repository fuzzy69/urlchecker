#include <iostream>
#include <string>

#include "catch.hpp"

#include "common/useragentmanager.h"

using common::browser::UserAgentManager;

TEST_CASE("Test user agent manager", "[useragentmanager]")
{
    UserAgentManager::self().add_user_agent("Firefox");
    REQUIRE(UserAgentManager::self().get_random() == "Firefox");
    REQUIRE(UserAgentManager::self().count() == 1);
    UserAgentManager::self().add_user_agent("Chrome");
    REQUIRE(UserAgentManager::self().count() == 2);
}
