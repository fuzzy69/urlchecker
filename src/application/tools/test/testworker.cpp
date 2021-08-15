#include <optional>

#include <QDebug>
#include <QUrl>
#include <QThread>

#include "my/text.h"

#include "testworker.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"

using my::text::trim_whitespaces;

TestWorker::TestWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

void TestWorker::doWork(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
//    QString url = "http://httpbin.org/ip";
    QString url = "http://httpbin.org/headers";
    int rowId = inputData["rowId"].toInt();

    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toStdString());
    // TODO: Better result status handling
    ResultStatus status((response.status_code == 200)? ResultStatus::OK : ResultStatus::FAILED);
    std::string text(trim_whitespaces(response.text));
    logMessage(QString::fromUtf8(text.c_str()));

    auto data = QMap<QString, QVariant>{
        {QString("toolId"), QVariant(Tools::TEST)},
        {QString("toolName"), QVariant("Test")},
        {QString("rowId"), QVariant(rowId)},
        {QString("URL"), QVariant(url)},
        {QString("Result"), QVariant(QString::fromUtf8(text.c_str()))},
        {QString("Details"), QVariant("")}
    };

    Q_EMIT Worker::result(data);
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
