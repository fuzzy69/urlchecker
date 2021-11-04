#include <optional>
#include <string>

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QUrl>

#include "../../../common/text.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "common.h"
#include "scrapesitemapsworker.h"
#include "utilities.h"

using common::text::starts_with;

ScrapeSitemapskWorker::ScrapeSitemapskWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::SCRAPE_SITEMAPS;
}

ScrapeSitemapskWorker::~ScrapeSitemapskWorker()
{
}

void ScrapeSitemapskWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    const QDir downloadSitemapsDirectory(m_settings[SCRAPE_SITEMAPS_DIRECTORY].toString());
    const bool downloadSitemaps(m_settings[DOWNLOAD_SITEMAPS].toBool());
    // Trim URL to root URL
    QUrl rootUrl;
    rootUrl.setScheme(url.scheme());
    rootUrl.setHost(url.host());
    // TODO: Skip domain if already scraped
    QUrl robotsUrl(rootUrl);
    robotsUrl.setPath("/robots.txt");
    logMessage(QString("Scraping sitemaps from URL '%1'...").arg(robotsUrl.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(robotsUrl.toString().toStdString());
    std::string sitemapUrl("");
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    if (response.status_code == 200) {
        sitemapUrl = extract_sitemap_url(response.text);
        if (sitemapUrl == "") {
            status = ResultStatus::FAILED;
            details = QStringLiteral("Failed to locate sitemap file");
        } else {
            // Download sitemap
            if (downloadSitemaps) {
                QUrl sitemapFileUrl(sitemapUrl.c_str());
                logMessage(QString("Downloading sitemap '%1'...").arg(sitemapUrl.c_str()));
                if (!sitemapFileUrl.fileName().isEmpty()) {
                    const QString sitemapFilePath = downloadSitemapsDirectory.filePath(url.host() + "_" + sitemapFileUrl.fileName());
                    response = requests.download(sitemapUrl, sitemapFilePath.toStdString());
                    status = (response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED;
                    if (status == ResultStatus::OK) {
                        logMessage(QString("Successfully downloaded sitemap '%1' to '%2'").arg(sitemapFileUrl.toString(), sitemapFilePath));
                        details = QString("Downloaded to '%1'...").arg(sitemapFilePath);
                    } else {
                        logMessage(QString("Failed to download sitemap '%1'!").arg(sitemapFileUrl.toString()));
                        details = QString::fromUtf8(response.status_line.c_str());
                    }
                }
            }
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }

    auto data = QVariantMap {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("Website"), QVariant(rootUrl.toString()) },
        { QStringLiteral("Details"), QVariant(details) },

        { QString("URL"), QVariant(QString::fromUtf8(sitemapUrl.c_str())) },
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
