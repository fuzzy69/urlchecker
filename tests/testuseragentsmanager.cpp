#include <iostream>
#include <string>

#include <QString>
#include "catch.hpp"

#include "application/utils/useragentsmanager.h"


TEST_CASE( "Test UserAgentsManager class", "[useragentsmanager]" )
{
    UserAgentsManager<std::string>::instance().add_user_agent("Firefox");
    REQUIRE(UserAgentsManager<std::string>::instance().get_random() == "Firefox");
    REQUIRE(UserAgentsManager<std::string>::instance().count() == 1);

    UserAgentsManager<QString>::instance().add_user_agent("Opera");
    REQUIRE(UserAgentsManager<QString>::instance().get_random() == "Opera");
    REQUIRE(UserAgentsManager<QString>::instance().count() == 1);
}
