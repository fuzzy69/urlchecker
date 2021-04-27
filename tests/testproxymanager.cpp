#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/utils/proxymanager.h"
#include "application/utils/httpproxy.h"


TEST_CASE( "Test ProxyManager class", "[proxymanager]" )
{
    ProxyManager::instance().add_proxy(HttpProxy("127.0.0.1", 80));
    ProxyManager::instance().add_proxy(HttpProxy("127.0.0.2", 80));
    ProxyManager::instance().add_proxy(HttpProxy("127.0.0.3", 80));
    auto proxy = ProxyManager::instance().get_proxy();
    REQUIRE(proxy.port() == 80);
    REQUIRE(proxy.ip() != "127.0.0.4");
    REQUIRE(ProxyManager::instance().count() == 3);
}
