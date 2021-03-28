#include <QObject>
#include <QQueue>
#include <QDebug>

#include "worker.h"


Worker::Worker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) :
m_inputDataQueue(inputDataQueue), m_settings(settings)
{
}

void Worker::run()
{
}

void Worker::stop()
{
    m_running = false;
}
