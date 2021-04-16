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

#include "../libs/cpr/include/cpr/cpr.h"

#include "scrapeproxies.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"


ScrapeProxiesWorker::ScrapeProxiesWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, settings, parent)
{
}

void ScrapeProxiesWorker::doWork(const QVariantMap& inputData)
{
    static const int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    static const bool verifySsl = m_settings["verifySsl"].toBool();
    static QRegularExpression regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})\\s*:?\\s*(\\d{2,5})");
    
    cpr::Session session;
    session.SetVerifySsl(verifySsl);
    QUrl url(inputData["url"].toString());
    session.SetUserAgent(USER_AGENT);
    session.SetTimeout(timeout);
    session.SetUrl(cpr::Url(url.toString().toStdString()));
    auto r = session.Get();

    QRegularExpressionMatchIterator regexIter = regex.globalMatch(QString(r.text.c_str()));
    while (regexIter.hasNext())
    {
        QRegularExpressionMatch match = regexIter.next();
        if (match.hasMatch())
        {
            auto data = QMap<QString, QVariant>{
                {QString("toolId"), QVariant(Tools::SCRAPE_PROXIES)},
                {QString("toolName"), QVariant("Scrape Proxies")},
                
                {QString("rowId"), QVariant(inputData["rowId"].toInt())},
                {QString("Proxy"), QVariant(match.captured(0))},
                {QString("Source"), QVariant(url)},
                {QString("Status"), QVariant(QString::fromUtf8(r.status_line.c_str()))}
            };
            emit Worker::result(data);
        }
    }

}
