#include <filesystem>
#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/tools/scrapeproxies/utilities.h"


TEST_CASE("Test proxy tools functions", "[proxytools]")
{
    const std::string text = R"(
127.0.0.1 80
127.0.0.1:80
127.0.0.1-80
    )";
    auto proxies = extract_proxies(text);
    for (const auto& proxy_string : proxies)
    {
        std::cout << proxy_string << "\n";
    }
    REQUIRE(proxies.size() == 2);
}
