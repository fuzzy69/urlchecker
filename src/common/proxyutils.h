#pragma once

#include <vector>
#include <string>

#include <QList>
#include <QString>
#include <QUrl>

#include "httpproxy.h"


using common::network::HttpProxy;

namespace common
{
namespace network
{
QList<HttpProxy> loadProxiesFromFile(const QString& filePath);

void saveProxiesToFile(const QString& filePath, const QList<HttpProxy>& proxies);
}
}
