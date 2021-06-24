#include "testworker.h"

// #include <QObject>
// #include <QDebug>
// #include <QThread>
// #include <QQueue>
// #include <QMutex>
// #include <QApplication>

#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../tools/tools.h"
#include "../utils/requests.h"


TestWorker::TestWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void TestWorker::doWork(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
//    QString url = inputData["url"].toString();
    QString url = "http://httpbin.org/ip";
    int rowId = inputData["rowId"].toInt();

    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());
    // TODO: Better result status handling
    ResultStatus status((response.status_code == 200)? ResultStatus::OK : ResultStatus::FAILED);

    auto data = QMap<QString, QVariant>{
        {QString("toolId"), QVariant(Tools::TEST)},
        {QString("toolName"), QVariant("Test")},
        {QString("rowId"), QVariant(rowId)},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(QString::fromUtf8(response.text.c_str()))},
        {QString("Details"), QVariant("")}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::status(rowId, status);
}
