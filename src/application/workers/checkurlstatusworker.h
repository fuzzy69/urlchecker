#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class CheckUrlStatusWorker final : public Worker
{
    Q_OBJECT

public:
    explicit CheckUrlStatusWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent = nullptr);

protected:
    void doWork(const QVariantMap &inputData) override;
};

