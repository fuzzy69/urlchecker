#pragma once

#include <string>
#include <QObject>
#include <QVariantMap>

#include "cpr/cpr.h"

class Requests
{
public:
    explicit Requests(const QVariantMap& settings);

    cpr::Response get(const std::string& url);
    cpr::Response head(const std::string& url);

private:
    QVariantMap m_settings;
    int m_timeout;
    bool m_verifySsl;
    bool m_useProxies;
};
