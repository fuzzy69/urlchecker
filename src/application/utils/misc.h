#pragma once

#include <QList>
#include <QString>

#include "httpproxy.h"


QList<HttpProxy> loadProxiesFromFile(const QString& filePath);
void saveProxiesToFile(const QString& filePath, const QList<HttpProxy>& proxies);
