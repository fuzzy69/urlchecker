#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>

#include "libs/cpr/include/cpr/cpr.h"

#include "testworker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"


TestWorker::TestWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, settings, parent)
{
}

void TestWorker::run()
{
    const int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    const bool verifySsl = m_settings["verifySsl"].toBool();
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
        url = "https://httpbin.org/headers";
        auto headers = cpr::Header{
            {"user-agent", USER_AGENT}
        };
        cpr::Response r;
        if (verifySsl)
            r = cpr::Get(cpr::Url{url.toStdString()}, cpr::Timeout{timeout}, headers);
        else
            r = cpr::Get(cpr::Url{url.toStdString()}, cpr::Timeout{timeout}, headers, cpr::VerifySsl{0});            
        qDebug() << r.text.c_str();
        qDebug() << r.status_code;
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::TEST)},
            {QString("toolName"), QVariant("Test")},
            {QString("URL"), QVariant(url)},
            {QString("Result"), QVariant(QString::fromUtf8(r.text.c_str()))},
            {QString("Status"), QVariant(ResultStatus::OK)}
        };
        emit Worker::result(data);
    }

    emit Worker::finished();
}
