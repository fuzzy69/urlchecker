#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QQueue>


class Worker : public QObject
{
    Q_OBJECT
    
public:
    explicit Worker(QQueue<QVariantMap> &inputDataQueue, const QVariantMap &settings, QObject *parent = nullptr);

signals:
    void result(const QVariantMap &resultData);
    void finished();
    void requestStop();

public slots:
    virtual void run() = 0;
    void stop();

protected:
    bool m_running;
    QQueue<QVariantMap> m_inputDataQueue;
    QVariantMap m_settings;
};

