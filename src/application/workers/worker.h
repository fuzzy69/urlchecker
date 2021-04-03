#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>
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
    void run();
    void stop();

protected:
    virtual void doWork(const QVariantMap& inputData) = 0;

    bool m_running;
    QQueue<QVariantMap> m_inputDataQueue;
    QVariantMap m_settings;
    QMutex m_mutex;
};

