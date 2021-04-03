#include <QApplication>
#include <QObject>
#include <QQueue>
#include <QDebug>

#include "worker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"


Worker::Worker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) :
m_inputDataQueue(inputDataQueue), m_settings(settings)
{
}

void Worker::run()
{
    m_running = true;
    while (m_running)
    {
        m_mutex.lock();
        if (m_inputDataQueue.empty())
        {
            m_mutex.unlock();
            break;
        }
        auto inputData = m_inputDataQueue.dequeue();
        m_mutex.unlock();
        QApplication::processEvents();

        doWork(inputData);
    }

    emit Worker::finished();
}

void Worker::stop()
{
    m_running = false;
}
