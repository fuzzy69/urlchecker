#pragma once

#include <QObject>
#include <QVariantMap>
#include <string>

#include "cpr/cpr.h"

class Requests {
public:
    explicit Requests(const QVariantMap& settings);

    cpr::Response get(const std::string& url);
    cpr::Response head(const std::string& url);
    cpr::Response download(const std::string& url, const std::string& filePath);

private:
    QVariantMap m_settings;
    int m_timeout;
    bool m_verifySsl;
    bool m_useProxies;
};
