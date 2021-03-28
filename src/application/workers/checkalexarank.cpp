#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QUrl>
#include <QRegExp>
#include <QApplication>

#include "libs/cpr/include/cpr/cpr.h"

#include "checkalexarank.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"


CheckAlexaRankWorker::CheckAlexaRankWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, settings, parent)
{
}

void CheckAlexaRankWorker::run()
{
    int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    m_running = true;
    QRegExp regex("RANK=\"(\\d+)\"");
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

        QUrl url(inputData["url"].toString());
        QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
        auto headers = cpr::Header{
            {"user-agent", USER_AGENT}
        };
        QString rank("");
        cpr::Response r = cpr::Get(cpr::Url{alexaUrl.toStdString()}, cpr::Timeout{timeout}, headers);
        int pos = regex.indexIn(QString(r.text.c_str()));
        if (pos > -1)
        {
            rank = regex.cap(1);
        }
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::CHECK_ALEXA_RANK)},
            {QString("toolName"), QVariant("Check Alexa Rank")},

            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("status"), QVariant(static_cast<qlonglong>(r.status_code))},
            {QString("message"), QVariant(QString::fromUtf8(r.status_line.c_str()))},
            {QString("result"), QVariant(rank)},

            {QString("URL"), QVariant(url)},
            {QString("Rank"), QVariant(rank)},
            {QString("Status"), QVariant(QString::fromUtf8(r.status_line.c_str()))}
        };
        emit Worker::result(data);
    }

    emit Worker::finished();
}
