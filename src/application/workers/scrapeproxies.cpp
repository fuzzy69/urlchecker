#include "scrapeproxies.h"

#include <QObject>
#include <QUrl>

#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../tools/proxytools.h"
#include "../tools/tools.h"
#include "../utils/requests.h"

ScrapeProxiesWorker::ScrapeProxiesWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void ScrapeProxiesWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Scraping proxies from: '%1'...").arg(url));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());
    // TODO: Better result status handling
    ResultStatus status((response.status_code == 200)? ResultStatus::OK : ResultStatus::FAILED);

    for (const auto& proxy_string : extract_proxies(response.text))
    {
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::SCRAPE_PROXIES)},
            {QString("toolName"), QVariant("Scrape Proxies")},
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("Proxy"), QVariant(QString::fromUtf8(proxy_string.c_str()))},
            {QString("Source"), QVariant(url)},
            {QString("Details"), QVariant("")}
        };
        Q_EMIT Worker::result(data);
    }
    Q_EMIT Worker::status(rowId, status);
}
