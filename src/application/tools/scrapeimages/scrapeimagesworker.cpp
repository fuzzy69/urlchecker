﻿#include <optional>
#include <string>
#include <unordered_set>

#include <QUrl>
#include <QDebug>

#include "my/text.h"

#include "scrapeimagesworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"

using my::text::starts_with;

ScrapeImagesWorker::ScrapeImagesWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent), m_dom(std::make_unique<SimpleDOM>()), m_tidy(std::make_unique<TidyHtml>())
{
}

ScrapeImagesWorker::~ScrapeImagesWorker()
{

}

void ScrapeImagesWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Scraping images from: '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    std::unordered_set<std::string> images;
    for (auto& element : m_dom->select_all("//img[@src]"))
    {
        std::string image = element.attribute("src");
        if (!starts_with(image, "http") or images.count(image) > 0)
            continue;
        images.insert(image);
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::SCRAPE_IMAGES)},
            {QString("toolName"), QVariant("Scrape Images")},
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("Image Source"), QVariant(QString::fromUtf8(image.c_str()))},
            {QString("Source"), QVariant(url)},
            {QString("Details"), QVariant("")}
        };
        Q_EMIT Worker::result(data);
    }

    auto status = ResultStatus::OK;
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}