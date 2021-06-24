#pragma once

#include "worker.h"

class TestWorker final: public Worker
{
    Q_OBJECT
    
public:
    explicit TestWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};
