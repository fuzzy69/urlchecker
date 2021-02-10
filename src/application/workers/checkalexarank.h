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
    explicit CheckAlexaRankWorker(QQueue<QMap<QString, QVariant>> &inputDataQueue, QObject *parent = nullptr);

Q_SIGNALS:
    void result(const QMap<QString, QVariant> &resultData);
    void finished();

public Q_SLOTS:
    void run() override;

private:
    QMutex m_mutex;
};


