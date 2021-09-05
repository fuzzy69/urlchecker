#include <optional>
#include <string>

#include <QDebug>
#include <QThread>
#include <QUrl>

#include "my/text.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"
#include "../tools.h"
#include "worker.h"

ScrapeMetaWorker::ScrapeMetaWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
    , m_dom(std::make_unique<SimpleDOM>())
    , m_tidy(std::make_unique<TidyHtml>())
{
    m_toolId = Tools::SCRAPE_META;
}

ScrapeMetaWorker::~ScrapeMetaWorker()
{
}

void ScrapeMetaWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    logMessage(QString("Scraping meta data from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral(""));
    QString title(""), description(""), keywords("");
    if (status == ResultStatus::OK) {
        std::string html = m_tidy->process(response.text);
        m_dom->from_string(html);
        auto element = m_dom->select_first("//title");
        if (element)
            title = QString::fromUtf8(element->text().c_str());
        element = m_dom->select_first("//meta[@name='description']");
        if (element)
            description = QString::fromUtf8(element->attribute("content").c_str());
        element = m_dom->select_first("//meta[@name='keywords']");
        if (element)
            keywords = QString::fromUtf8(element->attribute("content").c_str());
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }
    auto data = QMap<QString, QVariant> {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("URL"), inputData[FIELD_URL] },
        { QStringLiteral("Details"), QVariant(details) },

        { QStringLiteral("Title"), QVariant(title) },
        { QStringLiteral("Description"), QVariant(description) },
        { QStringLiteral("Keywords"), QVariant(keywords) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
