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
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
    logMessage(QString("Checking Alexa rank for '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(alexaUrl.toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    QString rank("");
    if (status == ResultStatus::OK) {
        std::optional<int> rank_result = extract_alexa_rank(response.text);
        if (rank_result) {
            status = ResultStatus::OK;
            rank = QString::number(rank_result.value());
        } else {
            details = QStringLiteral("Failed to extract rank value!");
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }

    auto data = QVariantMap {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("URL"), inputData[FIELD_URL] },
        { QStringLiteral("Details"), QVariant(details) },

        { QString("Rank"), QVariant(rank) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
