#include <QObject>
// #include <QDebug>
// #include <QThread>
#include <QQueue>
// #include <QMutex>

#include "worker.h"

Worker::Worker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    QObject(parent), m_running(false), m_inputDataQueue(inputDataQueue)
{
}

void Worker::run()
{
}
