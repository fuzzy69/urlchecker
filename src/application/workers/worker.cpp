#include <QObject>
#include <QQueue>
#include <QDebug>

#include "worker.h"


Worker::Worker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    QObject(parent), m_running(false), m_inputDataQueue(inputDataQueue)
{
}

void Worker::run()
{
}

void Worker::stop()
{
    m_running = false;
}
