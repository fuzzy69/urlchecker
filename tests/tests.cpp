#include <string>

#include "catch.hpp"

#include "application/utils/httpproxy.h"


TEST_CASE( "1: HttpProxy", "[httpproxy]" ) {
    auto httpProxy = HttpProxy::fromText("xxx");
    REQUIRE( httpProxy.ip() == "127.0.0.1" );
    REQUIRE( httpProxy.port() != 81 );
}
