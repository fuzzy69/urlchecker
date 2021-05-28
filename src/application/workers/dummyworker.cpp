#include "dummyworker.h"

#include <QThread>

#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../tools/tools.h"


DummyWorker::DummyWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void DummyWorker::doWork(const QVariantMap& inputData)
{
//     static const int timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
//     static const bool verifySsl = m_settings["verifySsl"].toBool();

    QString url = inputData["url"].toString();
    QThread::sleep(2);
    auto data = QVariantMap
    {
        {QString("toolId"), QVariant(Tools::DUMMY)},
        {QString("toolName"), QVariant("Dummy")},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant("OK")},
//         {QString("Status"), QVariant(ResultStatus::OK)}
    };

    Q_EMIT Worker::result(data);
}
