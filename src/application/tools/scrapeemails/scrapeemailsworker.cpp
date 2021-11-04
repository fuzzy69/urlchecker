#include <optional>
#include <string>

#include <QDebug>
#include <QThread>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../tools.h"
#include "scrapeemailsworker.h"
#include "utilities.h"

ScrapeEmailsWorker::ScrapeEmailsWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
{
    m_toolId = Tools::SCRAPE_EMAILS;
}

void ScrapeEmailsWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    logMessage(QString("Scraping emails from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral(""));
    if (status == ResultStatus::OK) {
        std::unordered_set<std::string> emails;
        for (const auto& email : extract_emails(response.text)) {
            if (emails.count(email) > 0)
                continue;
            emails.insert(email);
            auto data = QMap<QString, QVariant> {
                { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
                { QStringLiteral("Source"), inputData[FIELD_URL] },
                { QStringLiteral("Details"), QVariant(details) },

                { QStringLiteral("Email"), QVariant(QString::fromUtf8(email.c_str())) }
            };
            Q_EMIT Worker::result(m_toolId, data);
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }

    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
