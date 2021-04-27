#pragma once

#include <string>

#include <QList>
#include <QString>
#include <QUrl>

#include "httpproxy.h"


QList<HttpProxy> loadProxiesFromFile(const QString& filePath);
void saveProxiesToFile(const QString& filePath, const QList<HttpProxy>& proxies);

QList<QUrl> loadUrlsFromFile(const QString& filePath);
void saveUrlsToFile(const QString& filePath, const QList<QUrl>& urls);

bool is_valid_url(const std::string& url);
