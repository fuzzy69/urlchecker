#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>
#include <QDebug>

#include "checkurlstatusworker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../tools/tools.h"
#include "../utils/requests.h"


CheckUrlStatusWorker::CheckUrlStatusWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void CheckUrlStatusWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    int rowId = inputData["rowId"].toInt();

    Requests requests(m_settings);
    cpr::Response response = requests.head(url.toStdString());
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);

    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::CHECK_URL_STATUS)},
        {QString("toolName"), QVariant("Check URL Status")},

        {QString("rowId"), QVariant(rowId)},
        {QString("status"), QVariant(static_cast<qlonglong>(response.status_code))},
        {QString("message"), QVariant(QString::fromUtf8(response.status_line.c_str()))},

        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(static_cast<qlonglong>(response.status_code))},
//         {QString("Status"), QVariant(ResultStatus::OK)}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::status(rowId, ResultStatus::OK);
}
