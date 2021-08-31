#include <optional>
#include <string>

#include <QDebug>
#include <QThread>
#include <QUrl>

#include "my/text.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "scrapephonenumbersworker.h"
#include "utilities.h"

ScrapePhoneNumbersWorker::ScrapePhoneNumbersWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::SCRAPE_PHONE_NUMBERS;
}

void ScrapePhoneNumbersWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Scraping phone numbers from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    // TODO: Better result status handling
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));

    for (const auto& item : extract_phone_numbers(response.text)) {
        auto data = QMap<QString, QVariant> {
            { QString("rowId"), QVariant(inputData["rowId"].toInt()) },
            { QString("Phone Number"), QVariant(QString::fromUtf8(item.c_str())) },
            { QString("Source"), QVariant(url) },
            { QString("Details"), QVariant("") }
        };
        Q_EMIT Worker::result(m_toolId, data);
    }
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}
