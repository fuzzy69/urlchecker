#pragma once

#include <memory>

#include "../../core/worker.h"

class ScrapeSitemapskWorker final : public Worker {
    Q_OBJECT

public:
    ScrapeSitemapskWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent = nullptr);
    ~ScrapeSitemapskWorker();

protected:
    void doWork(const QVariantMap& inputData) override;
};
