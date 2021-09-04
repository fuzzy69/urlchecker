#include <optional>

#include <QDebug>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "scrapeproxiesworker.h"
#include "utilities.h"

ScrapeProxiesWorker::ScrapeProxiesWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::SCRAPE_PROXIES;
}

void ScrapeProxiesWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QString url(inputData[FIELD_URL].toString());
    logMessage(QString("Scraping proxies from: '%1'...").arg(url));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    for (const auto& proxy_string : extract_proxies(response.text)) {
        auto data = QMap<QString, QVariant> {
            { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
            { QStringLiteral("Source"), inputData[FIELD_URL] },
            { QStringLiteral("Details"), QVariant(details) },

            { QString("Proxy"), QVariant(QString::fromUtf8(proxy_string.c_str())) }
        };
        Q_EMIT Worker::result(m_toolId, data);
    }
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
