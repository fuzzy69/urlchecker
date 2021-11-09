#include <QDateTime>
#include <QDebug>
#include <QProgressBar>

#include "../texts.h"
#include "settings.h"
#include "thread.h"
#include "worker.h"
#include "workermanager.h"

WorkerManager::WorkerManager(QObject* parent)
    : QObject(parent)
    , m_threads(QList<Thread*>())
    , m_workers(QList<Worker*>())
    , m_inputDataQueue(QQueue<QMap<QString, QVariant>>())
    , m_mutex(QMutex())
    , m_itemsSuccessfullyDone(0)
    , m_itemsDone(0)
    , m_totalItems(0)
    , m_jobStartTimestamp(0)
    , m_jobEndTimestamp(0)
{
    setObjectName("WorkerManager");
}

void WorkerManager::init()
{
    m_totalItems = m_inputDataQueue.size();
    m_itemsSuccessfullyDone = 0;
    m_itemsDone = 0;
    m_jobStartTimestamp = QDateTime::currentSecsSinceEpoch();
    m_jobEndTimestamp = 0;
    m_threads.clear();
    m_workers.clear();
}

void WorkerManager::startJob(QList<Worker*> workers)
{
    init();
    for (int i = 0; i < workers.size(); ++i) {
        Worker* worker = workers.at(i);
        auto thread = new Thread;
        m_threads.append(thread);
        m_workers.append(worker);
        m_workers[i]->moveToThread(m_threads[i]);

        connect(thread, &Thread::started, worker, &Worker::run);
        connect(thread, &Thread::finished, thread, &Thread::deleteLater);
        connect(worker, &Worker::result, this, &WorkerManager::result);
        connect(worker, &Worker::status, this, &WorkerManager::status);
        connect(worker, &Worker::itemDone, this, &WorkerManager::onItemDone);
        connect(worker, &Worker::finished, thread, &Thread::quit);
        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
        connect(worker, &Worker::finished, [this] {
            m_jobEndTimestamp = QDateTime::currentSecsSinceEpoch();
        });
        connect(worker, &Worker::requestStop, worker, &Worker::stop);
        connect(worker, &Worker::log, this, &WorkerManager::log);
    }

    Q_EMIT jobStarted();
    for (int i = 0; i < workers.size(); ++i) {
        m_threads[i]->start();
    }
}

void WorkerManager::stopJob()
{
    for (Worker* worker : m_workers) {
        if (worker) {
            Q_EMIT worker->requestStop();
        }
    }
    Q_EMIT jobStopped();
    Q_EMIT jobStopRequested();
}

void WorkerManager::onItemDone(bool itemSuccess)
{
    ++m_itemsDone;
    if (itemSuccess)
        ++m_itemsSuccessfullyDone;
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    Q_EMIT progress(m_itemsSuccessfullyDone, m_itemsDone, m_totalItems, progresPercentage);
}

qint64 WorkerManager::jobRuntime()
{
    const qint64 runtime = (m_jobEndTimestamp == 0) ? QDateTime::currentSecsSinceEpoch() - m_jobStartTimestamp : m_jobEndTimestamp - m_jobStartTimestamp;

    return runtime;
}
