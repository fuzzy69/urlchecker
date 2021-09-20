#include <optional>
#include <string>
#include <unordered_set>
#include <qglobal.h>
#ifdef Q_OS_WIN
#include <ciso646>
#endif
#include <QDebug>
#include <QUrl>

#include "my/text.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"
#include "../tools.h"
#include "common.h"
#include "scrapelinksworker.h"
#include "utilities.h"

using my::text::starts_with;

ScrapeLinkskWorker::ScrapeLinkskWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
    , m_dom(std::make_unique<SimpleDOM>())
    , m_tidy(std::make_unique<TidyHtml>())
{
    m_toolId = Tools::SCRAPE_LINKS;
}

ScrapeLinkskWorker::~ScrapeLinkskWorker()
{
}

void ScrapeLinkskWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    ScrapeLinksStrategy strategy(static_cast<ScrapeLinksStrategy>(m_settings[SCRAPE_LINKS_STRATEGY].toInt()));
    logMessage(QString("Scraping links from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    std::unordered_set<std::string> links;
    if (status == ResultStatus::OK) {
        for (auto& element : m_dom->select_all("//a")) {
            // TODO: Validate link
            std::string link = element.attribute("href");
            if (!starts_with(link, "http") or links.count(link) > 0)
                continue;
            links.insert(link);
            QUrl linkUrl(link.c_str());
            switch (strategy) {
            case ScrapeLinksStrategy::INTERNAL_LINKS:
                if (linkUrl.host() != url.host())
                    continue;
                break;
            case ScrapeLinksStrategy::EXTERNAL_LINKS:
                if (linkUrl.host() == url.host())
                    continue;
                break;
            case ScrapeLinksStrategy::ALL_LINKS:
                // Ignore this case
                break;
            }

            auto data = QMap<QString, QVariant> {
                { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
                { QStringLiteral("Source"), inputData[FIELD_URL] },
                { QStringLiteral("Details"), QVariant(details) },

                { QStringLiteral("URL"), QVariant(QString::fromUtf8(link.c_str())) }
            };
            Q_EMIT Worker::result(m_toolId, data);
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }

    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
