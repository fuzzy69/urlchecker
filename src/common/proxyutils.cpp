#include "proxyutils.h"
#include "file.h"

using common::filesystem::File;
// using my::network::HttpProxy;

namespace common {
namespace network {
    QList<HttpProxy> loadProxiesFromFile(const QString& filePath)
    {
        QList<HttpProxy> proxies;
        for (QString& line : File::readTextLines(filePath)) {
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
}
}
