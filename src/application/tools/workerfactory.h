#pragma once

#include <QVariantMap>

class QMutex;
class Worker;

Worker* workerFactory(int id, int workerId, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings);
