#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class DummyWorker : public Worker
{
    Q_OBJECT
    
public:
    explicit DummyWorker(QQueue<QMap<QString, QVariant>> &inputDataQueue, QObject *parent = nullptr);

Q_SIGNALS:
    void result(const QMap<QString, QVariant> &resultData);
    void finished();

public Q_SLOTS:
    void run() override;

private:
    QMutex m_mutex;
};
