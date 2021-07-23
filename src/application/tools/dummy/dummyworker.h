#pragma once

#include "../../core/worker.h"

class DummyWorker final: public Worker
{
    Q_OBJECT
    
public:
    explicit DummyWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};
