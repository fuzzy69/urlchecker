#include <regex>

#include "file.h"
#include "urlutils.h"

using common::filesystem::File;

namespace common {
namespace url {
    QList<QString> loadUrlsFromFile(const QString& filePath)
    {
        QList<QString> urls;
        for (QString& line : File::readTextLines(filePath)) {
            line = line.trimmed();
            if (is_valid_url(line.toStdString()))
                urls.append(line);
        }

        return urls;
    }

    void saveUrlsToFile(const QString& filePath, const QList<QUrl>& urls)
    {
    }

    /// Source: https://mathiasbynens.be/demo/url-regex
    bool is_valid_url(const std::string& url)
    {
        // TODO: Improve URL matching regex
        const std::regex regex(R"((https?|ftp)://(-\.)?([^\s/?\.#]+\.?)+(/[^\s]*)?$)");

        return std::regex_match(url, regex);
    }
}
}
