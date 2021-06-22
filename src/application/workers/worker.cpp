#include "worker.h"

#include <QApplication>
#include <QMutex>

Worker::Worker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) :
m_running(false), m_inputDataQueue(inputDataQueue), m_mutex(mutex), m_settings(settings)
{
    Q_UNUSED(parent)
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

    emit Worker::finished();
}

void Worker::stop()
{
    m_running = false;
}
