#include <optional>
#include <string>

#include <QThread>
#include <QUrl>
#include <QDebug>

#include "my/text.h"

#include "scrapesitemapsworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"
//#include "../../utils/simpledom.h"
//#include "../../utils/tidyhtml.h"
#include "utilities.h"

using my::text::starts_with;

ScrapeSitemapskWorker::ScrapeSitemapskWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

ScrapeSitemapskWorker::~ScrapeSitemapskWorker()
{

}

void ScrapeSitemapskWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();

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
    auto status = ResultStatus::FAILED;
    QString details;
    if (response.status_code == 200)
    {
        sitemapUrl = extract_sitemap_url(response.text);
        details = QStringLiteral("OK");
        status = ResultStatus::OK;
    }
    else
    {
        details = QStringLiteral("Failed to locate sitemap file");
    }
    if (sitemapUrl == "")
        details = QStringLiteral("Failed to locate sitemap file");
    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::SCRAPE_SITEMAPS)},
        {QString("toolName"), QVariant("Scrape Sitemaps")},
        {QString("rowId"), QVariant(inputData["rowId"].toInt())},
        {QString("URL"), QVariant(QString::fromUtf8(sitemapUrl.c_str()))},
        {QString("Website"), QVariant(rootUrl.toString())},
        {QString("Details"), QVariant(details)}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
