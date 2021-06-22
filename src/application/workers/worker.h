#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QQueue>

#include "resultstatus.h"

class QMutex;

class Worker : public QObject
{
    Q_OBJECT
    
public:
    explicit Worker(QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

Q_SIGNALS:
    void result(const QVariantMap &resultData);
    void status(const int rowId, const ResultStatus &resultStatus);
    void finished();
    void requestStop();

public Q_SLOTS:
    void run();
    void stop();

protected:
    virtual void doWork(const QVariantMap& inputData) = 0;

    bool m_running;
    QQueue<QVariantMap>* m_inputDataQueue;
    QMutex* m_mutex;
    QVariantMap m_settings;
};

