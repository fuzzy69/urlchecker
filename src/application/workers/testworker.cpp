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


TestWorker::TestWorker(QQueue< QVariantMap >* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, mutex, settings, parent)
{
}

void TestWorker::doWork(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
//    QString url = inputData["url"].toString();
    QString url = "http://httpbin.org/ip";

    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());

    auto data = QMap<QString, QVariant>{
        {QString("toolId"), QVariant(Tools::TEST)},
        {QString("toolName"), QVariant("Test")},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(QString::fromUtf8(response.text.c_str()))},
//         {QString("Status"), QVariant(ResultStatus::OK)}
    };

    Q_EMIT Worker::result(data);
}
