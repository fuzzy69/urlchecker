#pragma once

#include <memory>

#include "../../core/worker.h"

class SimpleDOM;
class TidyHtml;

class CheckSearchEngineIndexWorker final : public Worker {
    Q_OBJECT

public:
    CheckSearchEngineIndexWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent = nullptr);
    ~CheckSearchEngineIndexWorker() override;

protected:
    void doWork(const QVariantMap& inputData) override;

private:
    std::unique_ptr<SimpleDOM> m_dom;
    std::unique_ptr<TidyHtml> m_tidy;
};
