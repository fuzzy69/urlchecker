#include <iostream>
#include <string>

#include "catch.hpp"

#include "common/httpproxy.h"
#include "common/proxymanager.h"

using common::network::HttpProxy;
using common::network::ProxyManager;

TEST_CASE("Test ProxyManager class", "[proxymanager]")
{
    ProxyManager::self().add_proxy(HttpProxy("127.0.0.1", 80));
    ProxyManager::self().add_proxy(HttpProxy("127.0.0.2", 80));
    ProxyManager::self().add_proxy(HttpProxy("127.0.0.3", 80));
    auto proxy = ProxyManager::self().get_proxy();
    REQUIRE(proxy.port() == 80);
    REQUIRE(proxy.ip() != "127.0.0.4");
    REQUIRE(ProxyManager::self().count() == 3);
}
