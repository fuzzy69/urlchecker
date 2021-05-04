#include <QObject>
// #include <QDebug>
// #include <QThread>
// #include <QQueue>
// #include <QMutex>
#include <QUrl>
// #include <QRegularExpression>
// #include <QRegularExpressionMatch>
// #include <QRegularExpressionMatchIterator>
#include <QApplication>

#include "../libs/cpr/include/cpr/cpr.h"

#include "scrapeproxies.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"
#include "../tools/proxytools.h"
#include "../utils/requests.h"


ScrapeProxiesWorker::ScrapeProxiesWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void ScrapeProxiesWorker::doWork(const QVariantMap& inputData)
{
//     static const int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
//     static const bool verifySsl = m_settings["verifySsl"].toBool();
//     static QRegularExpression regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})\\s*:?\\s*(\\d{2,5})");

    QString url = inputData["url"].toString();

    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());

    for (const auto& proxy_string : extract_proxies(response.text))
    {
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::SCRAPE_PROXIES)},
            {QString("toolName"), QVariant("Scrape Proxies")},

            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("Proxy"), QVariant(QString::fromUtf8(proxy_string.c_str()))},
            {QString("Source"), QVariant(url)},
            {QString("Status"), QVariant(ResultStatus::OK)}
        };
        emit Worker::result(data);
    }
    // cpr::Session session;
    // session.SetVerifySsl(verifySsl);
    // QUrl url(inputData["url"].toString());
    // session.SetUserAgent(USER_AGENT);
    // session.SetTimeout(timeout);
    // session.SetUrl(cpr::Url(url.toString().toStdString()));
    // auto r = session.Get();

//     QRegularExpressionMatchIterator regexIter = regex.globalMatch(QString(response.text.c_str()));
//     while (regexIter.hasNext())
//     {
//         QRegularExpressionMatch match = regexIter.next();
//         if (match.hasMatch())
//         {
//             auto data = QMap<QString, QVariant>{
//                 {QString("toolId"), QVariant(Tools::SCRAPE_PROXIES)},
//                 {QString("toolName"), QVariant("Scrape Proxies")},
//                 
//                 {QString("rowId"), QVariant(inputData["rowId"].toInt())},
//                 {QString("Proxy"), QVariant(match.captured(0))},
//                 {QString("Source"), QVariant(url)},
//                 {QString("Status"), QVariant(QString::fromUtf8(response.status_line.c_str()))}
//             };
//             emit Worker::result(data);
//         }
//     }

}
