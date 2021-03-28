#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class CheckAlexaRankWorker : public Worker
{
    Q_OBJECT
    
public:
    CheckAlexaRankWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent = nullptr)
    ;

Q_SIGNALS:
    void result(const QVariantMap &resultData);
    void finished();

public Q_SLOTS:
    void run() override;

private:
    QMutex m_mutex;
};


