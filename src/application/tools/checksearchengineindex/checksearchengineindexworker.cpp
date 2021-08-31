#include <optional>

#include <QDebug>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"
#include "../tools.h"
#include "checksearchengineindexworker.h"
#include "utilities.h"

CheckSearchEngineIndexWorker::CheckSearchEngineIndexWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
    , m_dom(std::make_unique<SimpleDOM>())
    , m_tidy(std::make_unique<TidyHtml>())
{
    m_toolId = Tools::CHECK_SEARCH_ENGINE_INDEX;
}

CheckSearchEngineIndexWorker::~CheckSearchEngineIndexWorker()
{
}

void CheckSearchEngineIndexWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    QUrl searchUrl("https://www.google.com/search");
    searchUrl.setQuery("q=" + QUrl::toPercentEncoding("site:" + url.toString()));
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Checking search engine index for: '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(searchUrl.toString().toStdString());

    QString indexStatus("Not Indexed");
    auto status = ResultStatus::FAILED;
    QString details;

    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    std::string urlString = url.toString().toStdString();
    for (auto& element : m_dom->select_all("//div/a")) {
        qDebug() << element.attribute("href").c_str();
        if (element.attribute("href") == urlString) {
            indexStatus = QStringLiteral("Indexed");
            break;
        }
    }
    auto data = QVariantMap {
        { QString("rowId"), QVariant(inputData["rowId"].toInt()) },
        { QString("URL"), QVariant(url) },
        { QString("Index Status"), QVariant(indexStatus) },
        { QString("Details"), QVariant(details) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
