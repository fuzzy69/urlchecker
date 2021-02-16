#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QUrl>
#include <QRegExp>
#include <QApplication>

#include "checkalexarank.h"
#include "../config.h"
#include "libs/cpr/include/cpr/cpr.h"


CheckAlexaRankWorker::CheckAlexaRankWorker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    Worker(inputDataQueue, parent)
{
}

void CheckAlexaRankWorker::run()
{
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

//         QString url = inputData["url"].toString();
        QUrl url(inputData["url"].toString());
        QString alexaUrl("http://data.alexa.com/data?cli=10&url=" + url.host());
        qDebug() << url.host();
        auto headers = cpr::Header{
            {"user-agent", USER_AGENT}
        };
        QString rank("");
        cpr::Response r = cpr::Get(cpr::Url{alexaUrl.toStdString()}, cpr::Timeout{15 * 1000}, headers);
//         qDebug() << r.status_code;
//         qDebug() << r.text.c_str();
        int pos = regex.indexIn(QString(r.text.c_str()));
        if (pos > -1)
        {
            rank = regex.cap(1);
        }
        auto data = QMap<QString, QVariant>{
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("status"), QVariant(static_cast<qlonglong>(r.status_code))},
            {QString("message"), QVariant(QString::fromUtf8(r.status_line.c_str()))},
            {QString("result"), QVariant(rank)}
        };
//         emit result(data);
        emit Worker::result(data);
    }

//     emit finished();
    emit Worker::finished();
}

