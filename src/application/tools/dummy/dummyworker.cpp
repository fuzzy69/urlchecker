﻿#include <optional>

#include <QDebug>
#include <QUrl>
#include <QThread>

#include "dummyworker.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"


DummyWorker::DummyWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void DummyWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    int rowId = inputData["rowId"].toInt();

    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    QThread::sleep(1);

    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::DUMMY)},
        {QString("toolName"), QVariant("Dummy")},
        {QString("rowId"), QVariant(rowId)},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant("OK")},
        {QString("Details"), QVariant("")},
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, ResultStatus::OK);
}
