#include <optional>

#include <QUrl>
#include <QDebug>

#include "checkalexarankworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"

CheckAlexaRankWorker::CheckAlexaRankWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void CheckAlexaRankWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Checking URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(alexaUrl.toStdString());

    QString rank("");
    auto status = ResultStatus::FAILED;
    QString details;
    std::optional<int> rank_result = extract_alexa_rank(response.text);
    if (rank_result)
    {
        status = ResultStatus::OK;
        rank = QString::number(rank_result.value());
        details = QStringLiteral("OK");
    }
    else
    {
        details = QStringLiteral("Failed extracting rank value");
    }

    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::CHECK_ALEXA_RANK)},
        {QString("toolName"), QVariant("Check Alexa Rank")},
        {QString("rowId"), QVariant(inputData["rowId"].toInt())},
        {QString("URL"), QVariant(url)},
        {QString("Rank"), QVariant(rank)},
        {QString("Details"), QVariant(details)}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::status(rowId, status);
}
