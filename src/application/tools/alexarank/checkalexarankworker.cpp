#include <optional>

#include <QDebug>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "checkalexarankworker.h"
#include "utilities.h"

CheckAlexaRankWorker::CheckAlexaRankWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::CHECK_ALEXA_RANK;
}

void CheckAlexaRankWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Checking Alexa rank for '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(alexaUrl.toStdString());

    QString rank("");
    auto status = ResultStatus::FAILED;
    QString details;
    std::optional<int> rank_result = extract_alexa_rank(response.text);
    if (rank_result) {
        status = ResultStatus::OK;
        rank = QString::number(rank_result.value());
        details = QStringLiteral("OK");
    } else {
        details = QStringLiteral("Failed to extract rank value");
    }

    auto data = QVariantMap {
        { QString("rowId"), QVariant(inputData["rowId"].toInt()) },
        { QString("URL"), QVariant(url) },
        { QString("Rank"), QVariant(rank) },
        { QString("Details"), QVariant(details) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::status(rowId, status);
}
