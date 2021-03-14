#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>

#include "libs/cpr/include/cpr/cpr.h"

#include "../config.h"
#include "dummyworker.h"


DummyWorker::DummyWorker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    Worker(inputDataQueue, parent)
{
}

void DummyWorker::run()
{
    m_running = true;
    while (m_running)
    {
        qDebug() << m_running;
        m_mutex.lock();
        if (m_inputDataQueue.empty())
        {
            m_mutex.unlock();
            break;
        }
        auto inputData = m_inputDataQueue.dequeue();
        m_mutex.unlock();
        QApplication::processEvents();

        QThread::sleep(2);
        auto data = QMap<QString, QVariant>{
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("status"), QVariant(200)},
            {QString("message"), QVariant("OK")}
        };
        emit Worker::result(data);
    }

    emit Worker::finished();
}
