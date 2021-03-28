#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>
#include <QDebug>

#include "libs/cpr/include/cpr/cpr.h"

#include "checkurlstatusworker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"


CheckUrlStatusWorker::CheckUrlStatusWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, settings, parent)
{
}

void CheckUrlStatusWorker::run()
{
    int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    m_running = true;
    while (m_running)
    {
        m_mutex.lock();
        if (m_inputDataQueue.empty())
        {
            m_mutex.unlock();
            break;
        }
        auto inputData = m_inputDataQueue.dequeue();
        m_mutex.unlock();
        QApplication::processEvents();

        QString url = inputData["url"].toString();
        auto headers = cpr::Header{
            {"user-agent", USER_AGENT}
        };
        cpr::Response r = cpr::Head(cpr::Url{url.toStdString()}, cpr::Timeout{timeout}, headers);
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::CHECK_URL_STATUS)},
            {QString("toolName"), QVariant("Check URL Status")},

            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("status"), QVariant(static_cast<qlonglong>(r.status_code))},
            {QString("message"), QVariant(QString::fromUtf8(r.status_line.c_str()))},

            {QString("URL"), QVariant(url)},
            {QString("Result"), QVariant(static_cast<qlonglong>(r.status_code))},
            {QString("Status"), QVariant(ResultStatus::OK)}
        };
        emit Worker::result(data);
    }

    emit Worker::finished();
}
