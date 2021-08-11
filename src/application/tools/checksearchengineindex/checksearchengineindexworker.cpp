#include <optional>

#include <QUrl>
#include <QDebug>

#include "checksearchengineindexworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"

CheckSearchEngineIndexWorker::CheckSearchEngineIndexWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent), m_dom(std::make_unique<SimpleDOM>()), m_tidy(std::make_unique<TidyHtml>())
{
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
//    qDebug() << response.status_code;
//    qDebug() << response.text.c_str();

    QString indexStatus("Not Indexed");
    auto status = ResultStatus::FAILED;
    QString details;

    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    std::string urlString = url.toString().toStdString();
//    qDebug() << urlString.c_str();
//    qDebug() << url;
//    qDebug() << searchUrl;
    for (auto& element : m_dom->select_all("//div/a"))
    {
        qDebug() << element.attribute("href").c_str();
        if (element.attribute("href") == urlString)
        {
            indexStatus = QStringLiteral("Indexed");
            break;
        }
    }
    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::CHECK_SEARCH_ENGINE_INDEX)},
        {QString("toolName"), QVariant("Check Search Engine Index")},
        {QString("rowId"), QVariant(inputData["rowId"].toInt())},
        {QString("URL"), QVariant(url)},
        {QString("Index Status"), QVariant(indexStatus)},
        {QString("Details"), QVariant(details)}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
