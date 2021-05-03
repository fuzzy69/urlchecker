#pragma once

#include <QObject>


class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

namespace my
{
namespace network
{
class HttpClient : public QObject
{
    Q_OBJECT
    
public:
    explicit HttpClient(QObject *parent = nullptr);

    QNetworkReply* head(const QUrl& url);
    QNetworkReply* head(const QString& url);
    QString userAgent() const;
    void setUserAgent(const QString &userAgent);
    int timeout() const;
    void setTimeout(int seconds);

signals:
    void replyTimeout();
    void replyFinished(int statusCode, const QString &statusText, const QString &text);

protected slots:


protected:
    void onReplyTimeout();
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
    QTimer *m_replyTimeoutTimer;
    QNetworkReply *m_reply;
    int m_replyTimeout;
    QByteArray m_userAgent;
};
}
}
