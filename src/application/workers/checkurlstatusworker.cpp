#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>
#include <QDebug>

#include "../libs/cpr/include/cpr/cpr.h"

#include "checkurlstatusworker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"
#include "../utils/requests.h"


CheckUrlStatusWorker::CheckUrlStatusWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void CheckUrlStatusWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();

    Requests requests(m_settings);
    cpr::Response response = requests.head(url.toStdString());

    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::CHECK_URL_STATUS)},
        {QString("toolName"), QVariant("Check URL Status")},

        {QString("rowId"), QVariant(inputData["rowId"].toInt())},
        {QString("status"), QVariant(static_cast<qlonglong>(response.status_code))},
        {QString("message"), QVariant(QString::fromUtf8(response.status_line.c_str()))},

        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(static_cast<qlonglong>(response.status_code))},
        {QString("Status"), QVariant(ResultStatus::OK)}
    };

    emit Worker::result(data);
}
