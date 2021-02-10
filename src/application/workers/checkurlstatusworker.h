#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutex>

#include "worker.h"


class CheckUrlStatusWorker : public Worker
{
    Q_OBJECT
    
public:
    explicit CheckUrlStatusWorker(QQueue<QMap<QString, QVariant>> &inputDataQueue, QObject *parent = nullptr);

Q_SIGNALS:
    void result(const QMap<QString, QVariant> &resultData);
    void finished();

public Q_SLOTS:
    void run() override;

private:
//     bool m_running;
//     QQueue<QMap<QString, QVariant>> m_inputDataQueue;
    QMutex m_mutex;
};

