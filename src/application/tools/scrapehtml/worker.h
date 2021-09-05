#pragma once

#include <memory>

#include "../../core/worker.h"

class ScrapeHtmlWorker final : public Worker {
    Q_OBJECT

public:
    ScrapeHtmlWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent = nullptr);

protected:
    void doWork(const QVariantMap& inputData) override;
};
