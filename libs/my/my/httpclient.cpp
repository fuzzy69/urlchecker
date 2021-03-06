#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

#include "my/httpclient.h"


#ifndef USER_AGENT
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:70.0) Gecko/20100101 Firefox/70.0"
#endif

using my::network::HttpClient;

HttpClient::HttpClient(QObject* parent)
{
    Q_UNUSED(parent);
    m_networkManager = new QNetworkAccessManager(this);
    m_replyTimeoutTimer = new QTimer(this);

    connect(m_replyTimeoutTimer, &QTimer::timeout, this, &HttpClient::onReplyTimeout);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &HttpClient::onReplyFinished);

    setUserAgent(QString(USER_AGENT));
    setTimeout(20);
}

QNetworkReply * HttpClient::head(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", m_userAgent);
    m_reply = m_networkManager->head(request);
    m_replyTimeoutTimer->start(m_replyTimeout * 1000);

    // TODO: Finish this
    return nullptr;
}

QNetworkReply * HttpClient::head(const QString& url)
{
    return head(QUrl(url));
}

void HttpClient::onReplyTimeout()
{
    m_reply->abort();
}

void HttpClient::onReplyFinished(QNetworkReply* reply)
{
    if (m_replyTimeoutTimer->isActive())
        m_replyTimeoutTimer->stop();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString statusText = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    QString text = (reply->error() != QNetworkReply::NoError)? "" : reply->readAll();
    emit replyFinished(statusCode, statusText, text);
    reply->deleteLater();
}

void HttpClient::setTimeout(int seconds)
{
    m_replyTimeout = seconds;
}

int HttpClient::timeout() const
{
    return m_replyTimeout;
}

void HttpClient::setUserAgent(const QString& userAgent)
{
    m_userAgent = userAgent.toUtf8();
}

QString HttpClient::userAgent() const
{
    return QString::fromUtf8(m_userAgent);
}
