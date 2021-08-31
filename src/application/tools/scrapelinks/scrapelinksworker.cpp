#include <optional>
#include <string>

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
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();
    ScrapeLinksStrategy strategy(static_cast<ScrapeLinksStrategy>(m_settings[SCRAPE_LINKS_STRATEGY].toInt()));

    logMessage(QString("Scraping links from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    for (auto& element : m_dom->select_all("//a")) {
        // TODO: Validate link
        std::string link = element.attribute("href");
        // TODO: Better URL filtering
        if (!starts_with(link, "http"))
            continue;
        switch (strategy) {
        case ScrapeLinksStrategy::INTERNAL_LINKS:
            // Filter internal links
            break;
        case ScrapeLinksStrategy::EXTERNAL_LINKS:
            // Filter external links
            break;
        case ScrapeLinksStrategy::ALL_LINKS:
            //
            break;
        }

        auto data = QMap<QString, QVariant> {
            { QString("rowId"), QVariant(inputData["rowId"].toInt()) },
            { QString("URL"), QVariant(QString::fromUtf8(link.c_str())) },
            { QString("Source"), QVariant(url) },
            { QString("Details"), QVariant("") }
        };
        Q_EMIT Worker::result(m_toolId, data);
    }

    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, ResultStatus::OK);
}
