﻿#include <optional>
#include <string>

#include <QThread>
#include <QUrl>
#include <QDebug>

#include "my/text.h"

#include "scrapeemailsworker.h"
#include "utilities.h"
#include "../../core/resultstatus.h"
#include "../../config.h"
#include "../../constants.h"
#include "../tools.h"
#include "../../utils/requests.h"
#include "utilities.h"

ScrapeEmailsWorker::ScrapeEmailsWorker(int id, QQueue<QVariantMap> *inputDataQueue, QMutex* mutex, const QVariantMap &settings, QObject *parent) : Worker(id, inputDataQueue, mutex, settings, parent)
{
}

//ScrapeEmailsWorker::~ScrapeEmailsWorker()
//{

//}

void ScrapeEmailsWorker::doWork(const QVariantMap& inputData)
{
    QUrl url(inputData["url"].toString());
    int rowId = inputData["rowId"].toInt();

    logMessage(QString("Scraping emails from URL '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    // TODO: Better result status handling
    ResultStatus status((response.status_code == 200)? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
//    details = QStringLiteral("Failed to extract emails");

    for (const auto& email : extract_emails(response.text))
    {
        auto data = QMap<QString, QVariant>{
            {QString("toolId"), QVariant(Tools::SCRAPE_EMAILS)},
            {QString("toolName"), QVariant("Scrape Emails")},
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("Email"), QVariant(QString::fromUtf8(email.c_str()))},
            {QString("Source"), QVariant(url)},
            {QString("Details"), QVariant("")}
        };
        Q_EMIT Worker::result(data);
    }
    Q_EMIT Worker::itemDone();
    Q_EMIT Worker::status(rowId, status);
}