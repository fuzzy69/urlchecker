#include "worker.h"

#include <QApplication>
#include <QDateTime>
#include <QMutex>

Worker::Worker(int id, QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) :
m_id(id), m_running(false), m_inputDataQueue(inputDataQueue), m_mutex(mutex), m_settings(settings)
{
    Q_UNUSED(parent)
}

void Worker::logMessage(const QString &message)
{
    Q_EMIT Worker::log(QString("[%1] %2 <Worker %3> %4")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg("INFO")
        .arg(m_id)
        .arg(message)
    );
}

void Worker::run()
{
    m_running = true;
    while (m_running)
    {
        m_mutex->lock();
        if (m_inputDataQueue->empty())
        {
            m_mutex->unlock();
            break;
        }
        auto inputData = m_inputDataQueue->dequeue();
        m_mutex->unlock();
        QApplication::processEvents();

        doWork(inputData);
    }

    Q_EMIT Worker::finished();
    logMessage("Worker finished.");
}

void Worker::stop()
{
    m_running = false;
}
