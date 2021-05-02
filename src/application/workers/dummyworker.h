#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class DummyWorker : public Worker
{
    Q_OBJECT
    
public:
    explicit DummyWorker(QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};
