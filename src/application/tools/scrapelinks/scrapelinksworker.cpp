#include <optional>
#include <string>

#include <QUrl>
#include <QDebug>

#include "my/text.h"

#include "scrapelinksworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"

using my::text::starts_with;

ScrapeLinkskWorker::ScrapeLinkskWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void ScrapeLinkskWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Scraping links from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    TidyHtml tidy_html;
    std::string html = tidy_html.process(response.text);
    SimpleDOM dom;
    dom.from_string(html);
    for (auto& element : dom.select_all("//a"))
    {
        // TODO: Validate link
        std::string link = element.attribute("href");
        // TODO: Better URL filtering
        if (!starts_with(link, "http"))
            continue;
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::SCRAPE_LINKS)},
            {QString("toolName"), QVariant("Scrape Links")},
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("URL"), QVariant(QString::fromUtf8(link.c_str()))},
            {QString("Source"), QVariant(url)},
            {QString("Details"), QVariant("")}
        };
        Q_EMIT Worker::result(data);
    }

    auto status = ResultStatus::OK;
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
