#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/utils/proxymanager.h"
#include "application/utils/httpproxy.h"


TEST_CASE( "Test ProxyManager class", "[proxymanager]" )
{
    ProxyManager proxyManager;
    proxyManager.add_proxy(HttpProxy("127.0.0.1", 80));
    proxyManager.add_proxy(HttpProxy("127.0.0.2", 80));
    proxyManager.add_proxy(HttpProxy("127.0.0.3", 80));
    auto proxy = proxyManager.get_proxy();
    REQUIRE(proxy.port() == 80);
    REQUIRE(proxy.ip() != "127.0.0.4");
    REQUIRE(proxyManager.count() == 3);
}
