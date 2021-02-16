#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QUrl>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QApplication>

#include "scrapeproxies.h"
#include "../config.h"
#include "libs/cpr/include/cpr/cpr.h"


ScrapeProxiesWorker::ScrapeProxiesWorker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    Worker(inputDataQueue, parent)
{
}

void ScrapeProxiesWorker::run()
{
    m_running = true;
    QRegularExpression regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})\\s*:?\\s*(\\d{2,5})");
    cpr::Session session;
    session.SetVerifySsl(false);
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
        session.SetUserAgent(USER_AGENT);
        session.SetTimeout(15 * 1000);
        session.SetUrl(cpr::Url(url.toString().toStdString()));
        auto r = session.Get();

        QRegularExpressionMatchIterator regexIter = regex.globalMatch(QString(r.text.c_str()));
        while (regexIter.hasNext())
        {
            QRegularExpressionMatch match = regexIter.next();
            if (match.hasMatch())
            {
                qDebug() << match.captured(0);
                auto data = QMap<QString, QVariant>{
                    {QString("rowId"), QVariant(inputData["rowId"].toInt())},
                    {QString("status"), QVariant(static_cast<qlonglong>(r.status_code))},
                    {QString("message"), QVariant(QString::fromUtf8(r.status_line.c_str()))},
                    {QString("result"), QVariant(match.captured(0))}
                };
                emit Worker::result(data);
            }
        }
    }

    emit Worker::finished();
}
