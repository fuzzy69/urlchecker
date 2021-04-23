#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/utils/httpproxy.h"


TEST_CASE( "Test Public HttpProxy", "[httpproxy]" )
{
    auto httpProxyResult = HttpProxy::from_text("127.0.0.1:80");
    REQUIRE(httpProxyResult);
    auto httpProxy = httpProxyResult.value();
    REQUIRE(httpProxy.is_public());
    REQUIRE(httpProxy.ip() == "127.0.0.1");
    REQUIRE(httpProxy.port() == 80);
    REQUIRE(httpProxy == HttpProxy("127.0.0.1", 80));
}

TEST_CASE( "Test Private HttpProxy", "[httpproxy]" )
{
    auto httpProxyResult = HttpProxy::from_text("127.0.0.1:80:user:pass");
    REQUIRE(httpProxyResult);
    auto httpProxy = httpProxyResult.value();
    REQUIRE(!httpProxy.is_public());
    REQUIRE(httpProxy.ip() == "127.0.0.1");
    REQUIRE(httpProxy.port() == 80);
    REQUIRE(httpProxy.username() == "user");
    REQUIRE(httpProxy.password() == "pass");
    REQUIRE(httpProxy == HttpProxy("127.0.0.1", 80, "user", "pass"));
}
