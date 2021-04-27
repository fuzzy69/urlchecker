#include <iostream>
#include <string>

#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "catch.hpp"

#include "application/utils/requests.h"


TEST_CASE( "Test Requests class", "[requests]" )
{
    QVariantMap settings;
    settings.insert(QString("timeout"), QVariant(15));
    settings.insert(QString("verifySsl"), QVariant(false));
    settings.insert(QString("useProxies"), QVariant(false));
    Requests requests(settings);
    cpr::Response response = requests.get("http://httpbin.org/headers");
    REQUIRE(response.status_code == 200);
}
