#include <QProgressBar>
#include <QDebug>

#include "applicationbridge.h"
#include "settings.h"
#include "table.h"
#include "thread.h"
#include "toolsmanager.h"
#include "worker.h"
#include "workermanager.h"
#include "../widgets/logwidget.h"
#include "../widgets/tableswidget.h"
#include "../texts.h"
#include "../tools/workerfactory.h"

WorkerManager::WorkerManager(QObject *parent) : QObject(parent), m_threads(QList<Thread*>()), m_workers(QList<Worker*>()), m_inputDataQueue(QQueue<QMap<QString, QVariant>>()), m_mutex(QMutex()), m_currentSettings(QVariantMap()), m_itemsDone(0), m_totalItems(0)
{
    setObjectName("WorkerManager");
}

void WorkerManager::init()
{
    Table* inputTable = ApplicationBridge::instance().tablesWidget()->inputTable();
    m_itemsDone = 0;
    m_totalItems = inputTable->rowCount();
    m_threads.clear();
    m_workers.clear();

}

void WorkerManager::startJob()
{
    init();
//    auto& currentTool = ToolsManager::instance().currentTool();
    auto& currentTool = ToolsManager::instance().currentTool();
    Table* inputTable = ApplicationBridge::instance().tablesWidget()->inputTable();
    for (int i = 0; i < inputTable->rowCount(); ++i)
    {
        auto url = inputTable->cell(i, 0).toString();
        m_inputDataQueue.enqueue({
            {QString("rowId"), QVariant(i)},
            {QString("url"), QVariant(url)}
        });
    }
    const int parallelTasks = Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt();
    assert(parallelTasks > 0);
    auto logWidget = ApplicationBridge::instance().logWidget();
    m_currentSettings = Settings::instance().get();
    for (int i = 0; i < parallelTasks;++i)
    {
        int workerId(i+ 1);
        auto thread = new Thread;
        Worker *worker;

        // TODO: Improve tool switching logic
//        QVariantMap settings;
//        settings.insert(QString("timeout"), Settings::instance().value(QStringLiteral(TEXT_TIMEOUT)));
//        settings.insert(QString("useProxies"), Settings::instance().value(QStringLiteral(TEXT_USE_PROXIES)));
//        worker = workerFactory(currentTool.id(), workerId, &m_inputDataQueue, &m_mutex, settings);
        worker = workerFactory(currentTool.id(), workerId, &m_inputDataQueue, &m_mutex, m_currentSettings);

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
        connect(worker, &Worker::finished, []{
            qDebug() << "Worker finished";
        });
        connect(worker, &Worker::requestStop, worker, &Worker::stop);
        connect(worker, &Worker::log, logWidget, &LogWidget::onLog);
    }

        Q_EMIT jobStarted();
        for (int i = 0; i < parallelTasks; ++i)
        {
            m_threads[i]->start();
        }
}

void WorkerManager::stopJob()
{
    for (Worker *worker: m_workers)
    {
        if (worker)
        {
            Q_EMIT worker->requestStop();
        }
    }
    Q_EMIT jobStopped();
}

void WorkerManager::onItemDone()
{
    ++m_itemsDone;
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    ApplicationBridge::instance().progressBar()->setValue(progresPercentage);
}
