#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class TestWorker final: public Worker
{
    Q_OBJECT
    
public:
    explicit TestWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};
