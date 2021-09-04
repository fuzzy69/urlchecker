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
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QString url(inputData[FIELD_URL].toString());
    logMessage(QString("Dummy job for URL '%1'...").arg(url));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    QThread::sleep(1);
    ResultStatus status(ResultStatus::OK);
    QString details(QStringLiteral("Some details ..."));
    auto data = QVariantMap {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("URL"), inputData[FIELD_URL] },
        { QStringLiteral("Details"), QVariant(details) },

        { QString("Result"), QVariant(QStringLiteral("OK")) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
