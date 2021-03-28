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
    explicit CheckUrlStatusWorker(QQueue<QVariantMap> &inputDataQueue, const QVariantMap &settings, QObject *parent = nullptr);

Q_SIGNALS:
    void result(const QVariantMap &resultData);
    void finished();

public Q_SLOTS:
    void run() override;

private:
    QMutex m_mutex;
};

