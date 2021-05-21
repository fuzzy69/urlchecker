#include <optional>

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QUrl>
#include <QRegExp>
#include <QApplication>

#include "checkalexarank.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"
#include "../tools/ranktools.h"
#include "../utils/requests.h"


CheckAlexaRankWorker::CheckAlexaRankWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void CheckAlexaRankWorker::doWork(const QVariantMap& inputData)
{
//     static QRegExp regex("RANK=\"(\\d+)\"");

    QUrl url(inputData["url"].toString());
    QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
//     QString rank("");

    Requests requests(m_settings);
    cpr::Response response = requests.get(alexaUrl.toStdString());

    QString rank("");
    auto status = ResultStatus::FAILED;
    QString message("Done");
    std::optional<int> rank_result = extract_alexa_rank(response.text);
    if (rank_result)
    {
        status = ResultStatus::OK;
        rank = QString::number(rank_result.value());
    }
    else
    {
        message = QString("Failed extracting rank value");
    }
    
//     int pos = regex.indexIn(QString(response.text.c_str()));
//     if (pos > -1)
//     {
//         rank = regex.cap(1);
//     }
    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::CHECK_ALEXA_RANK)},
        {QString("toolName"), QVariant("Check Alexa Rank")},

        {QString("rowId"), QVariant(inputData["rowId"].toInt())},
        {QString("status"), QVariant(static_cast<qlonglong>(response.status_code))},
//         {QString("message"), QVariant(status)},
        {QString("result"), QVariant(rank)},

        {QString("URL"), QVariant(url)},
        {QString("Rank"), QVariant(rank)},
//         {QString("Status"), QVariant(status)}
    };

    emit Worker::result(data);
}
