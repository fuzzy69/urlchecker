﻿#pragma once

#include <memory>

#include "../../core/worker.h"

class SimpleDOM;
class TidyHtml;

class ScrapeHtmlWorker final : public Worker {
    Q_OBJECT

public:
    ScrapeHtmlWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent = nullptr);
    ~ScrapeHtmlWorker() override;

protected:
    void doWork(const QVariantMap& inputData) override;

private:
    std::unique_ptr<SimpleDOM> m_dom;
    std::unique_ptr<TidyHtml> m_tidy;
};
