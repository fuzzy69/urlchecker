#pragma once

#include <string>

#include <QList>
#include <QString>
#include <QUrl>

namespace common {
namespace url {
    QList<QString> loadUrlsFromFile(const QString& filePath);

    void saveUrlsToFile(const QString& filePath, const QList<QUrl>& urls);

    bool is_valid_url(const std::string& url);
}
}
