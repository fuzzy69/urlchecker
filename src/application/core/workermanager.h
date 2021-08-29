#pragma once

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "../core/resultstatus.h"
#include "../tools/tools.h"

class Thread;
class Worker;

class Tool;

class WorkerManager final : public QObject
{
    Q_OBJECT

public:
    explicit WorkerManager(QObject *parent = nullptr);
    void init();

Q_SIGNALS:
    void jobStarted();
    void jobStopped();
//    void result(const QVariantMap &resultData);
    void result(Tools toolId, const QVariantMap &resultData);
    void status(const int rowId, const ResultStatus &resultStatus);

public Q_SLOTS:
    void startJob();
    void stopJob();
    void onItemDone();

private:
    QList<Thread*> m_threads;
    QList<Worker*> m_workers;
    QQueue<QVariantMap> m_inputDataQueue;
    QMutex m_mutex;
    QVariantMap m_currentSettings;

    int m_itemsDone;
    int m_totalItems;
};
