#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QApplication>

#include "testworker.h"
#include "resultstatus.h"
#include "../config.h"
#include "../constants.h"
#include "../core/tools.h"
#include "../utils/proxymanager.h"
#include "../utils/requests.h"
#include "../utils/useragentsmanager.h"


TestWorker::TestWorker(QQueue< QVariantMap >& inputDataQueue, const QVariantMap& settings, QObject* parent) : Worker(inputDataQueue, settings, parent)
{
}

void TestWorker::doWork(const QVariantMap& inputData)
{
    QString url = inputData["url"].toString();
    url = "http://httpbin.org/ip";

    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());

    auto data = QMap<QString, QVariant>{
        {QString("toolId"), QVariant(Tools::TEST)},
        {QString("toolName"), QVariant("Test")},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(QString::fromUtf8(response.text.c_str()))},
        {QString("Status"), QVariant(ResultStatus::OK)}
    };
    qDebug() << response.text.c_str();

    emit Worker::result(data);
}
