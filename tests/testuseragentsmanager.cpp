#include <iostream>
#include <string>

#include <QString>
#include "catch.hpp"

#include "application/utils/useragentsmanager.h"


TEST_CASE( "Test UserAgentsManager class", "[useragentsmanager]" )
{
    UserAgentsManager<std::string> userAgentsManager;
    userAgentsManager.add_user_agent("Firefox");
    REQUIRE(userAgentsManager.get_random() == "Firefox");
    REQUIRE(userAgentsManager.count() == 1);
    UserAgentsManager<QString> userAgentsManager2;
    userAgentsManager2.add_user_agent(QStringLiteral("Opera"));
    REQUIRE(userAgentsManager2.get_random() == QString("Opera"));
    REQUIRE(userAgentsManager2.count() == 1);
}
