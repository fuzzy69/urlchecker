#include <optional>

#include <QDebug>
#include <QThread>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "dummytool.h"
#include "dummyworker.h"

DummyWorker::DummyWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::DUMMY;
}

void DummyWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    int rowId = inputData["rowId"].toInt();

    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    QThread::sleep(1);

    auto data = QVariantMap {
        { QString("rowId"), QVariant(rowId) },
        { QString("URL"), QVariant(url) },
        { QString("Result"), QVariant("OK") },
        { QString("Details"), QVariant("") },
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, ResultStatus::OK);
}
