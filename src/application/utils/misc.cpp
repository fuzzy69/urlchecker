#include <regex>

#include "file.h"
#include "misc.h"
#include "text.h"


QList<HttpProxy> loadProxiesFromFile(const QString& filePath)
{
    QList<HttpProxy> proxies;
    for (QString& line : File::readTextLines(filePath))
    {
        auto httpProxyResult = HttpProxy::from_text(line.trimmed().toStdString());
        if (httpProxyResult && !proxies.contains(httpProxyResult.value()))
            proxies.append(httpProxyResult.value());
    }

    return proxies;
}

void saveProxiesToFile(const QString& filePath, const QList<HttpProxy>& proxies)
{
    QString text;
    // TODO: Finish this
    // for (const auto& proxy : proxies)
    // {
    //     text.append(QString::fromStdString(proxy));
    // }
    // File::writeTextFile(filePath, text);
}

QList<QUrl> loadUrlsFromFile(const QString& filePath)
{
    QList<QUrl> urls;
    for (QString& line : File::readTextLines(filePath))
    {
        line = line.trimmed();
        if (is_valid_url(line.toStdString()))
            urls.append(QUrl(line));
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
