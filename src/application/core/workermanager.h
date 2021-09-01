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

class WorkerManager final : public QObject {
    Q_OBJECT

public:
    explicit WorkerManager(QObject* parent = nullptr);
    void init();

Q_SIGNALS:
    void jobStarted();
    void jobStopped();
    void result(Tools toolId, const QVariantMap& resultData);
    void status(const int rowId, const ResultStatus& resultStatus);
    void progress(const int itemsSuccessfullyDone, const int itemsDone, const int totalItems, const double progressPercentage);

public Q_SLOTS:
    void startJob();
    void stopJob();
    void onItemDone(bool itemSuccess);
    qint64 jobRuntime();

private:
    QList<Thread*> m_threads;
    QList<Worker*> m_workers;
    QQueue<QVariantMap> m_inputDataQueue;
    QMutex m_mutex;
    QVariantMap m_currentSettings;

    int m_itemsSuccessfullyDone;
    int m_itemsDone;
    int m_totalItems;
    qint64 m_jobStartTimestamp;
    qint64 m_jobEndTimestamp;
};
