#include "checkalexarank.h"

#include <optional>

#include <QUrl>

#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../tools/ranktools.h"
#include "../tools/tools.h"
#include "../utils/requests.h"


CheckAlexaRankWorker::CheckAlexaRankWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void CheckAlexaRankWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());

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

    Q_EMIT Worker::result(data);
}
