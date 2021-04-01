#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class CheckAlexaRankWorker final: public Worker
{
    Q_OBJECT

public:
    CheckAlexaRankWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent = nullptr);

public Q_SLOTS:
    void run() override;

private:
    QMutex m_mutex;
};
