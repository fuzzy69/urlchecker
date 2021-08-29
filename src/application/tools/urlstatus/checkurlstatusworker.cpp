#include <optional>

#include <QUrl>
#include <QDebug>

#include "checkurlstatusworker.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"


CheckUrlStatusWorker::CheckUrlStatusWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::CHECK_URL_STATUS;
}

void CheckUrlStatusWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Checking URL status for '%1'...").arg(url));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.head(url.toStdString());

    auto data = QVariantMap
    {
        {QString("rowId"), QVariant(rowId)},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(static_cast<qlonglong>(response.status_code))},
        {QString("Details"), QVariant(QString::fromUtf8(response.status_line.c_str()))}
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, ResultStatus::OK);
}
