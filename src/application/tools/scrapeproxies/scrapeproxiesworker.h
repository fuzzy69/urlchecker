#pragma once

#include "../../core/worker.h"

class ScrapeProxiesWorker final: public Worker
{
    Q_OBJECT

public:
    explicit ScrapeProxiesWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};
