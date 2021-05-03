#pragma once

#include <vector>
#include <string>

#include <QList>
#include <QString>
#include <QUrl>

#include "my/httpproxy.h"


using my::network::HttpProxy;

namespace my
{
namespace network
{
QList<HttpProxy> loadProxiesFromFile(const QString& filePath);

void saveProxiesToFile(const QString& filePath, const QList<HttpProxy>& proxies);
}
}
