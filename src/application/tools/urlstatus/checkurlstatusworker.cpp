#include <optional>

#include <QDebug>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "checkurlstatusworker.h"

CheckUrlStatusWorker::CheckUrlStatusWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::CHECK_URL_STATUS;
}

void CheckUrlStatusWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QString url(inputData[FIELD_URL].toString());
    logMessage(QString("Checking URL status for '%1'...").arg(url));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.head(url.toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QString::fromUtf8(response.status_line.c_str()));
    auto data = QVariantMap {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("URL"), inputData[FIELD_URL] },
        { QStringLiteral("Details"), QVariant(details) },

        { QString("Result"), QVariant(static_cast<qlonglong>(response.status_code)) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
