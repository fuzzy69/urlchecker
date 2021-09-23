#include <optional>
#include <string>
#include <unordered_set>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QUrl>

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"
#include "../tools.h"
#include "common.h"
#include "my/text.h"
#include "utilities.h"
#include "worker.h"

using my::text::strip;

ScrapeHtmlWorker::ScrapeHtmlWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
    , m_dom(std::make_unique<SimpleDOM>())
    , m_tidy(std::make_unique<TidyHtml>())
{
    m_toolId = Tools::SCRAPE_HTML;
}

ScrapeHtmlWorker::~ScrapeHtmlWorker()
{
}

void ScrapeHtmlWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    const QDir downloadDirectory(m_settings[HTML_DIRECTORY].toString());
    ScrapeHtmlStrategy strategy(static_cast<ScrapeHtmlStrategy>(m_settings[SCRAPE_HTML_STRATEGY].toInt()));
    logMessage(QString("Scraping HTML from: '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    if (status == ResultStatus::OK) {
        if (strategy == ScrapeHtmlStrategy::FULL_HTML) {
            const QString fileName(QUrl::toPercentEncoding(url.toString()) + ".html");
            const QString filePath(downloadDirectory.filePath(fileName));
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                logMessage(QString("Downloading HTML to '%1'...").arg(filePath));
                QTextStream textStream(&file);
                textStream << QString::fromUtf8(response.text.c_str());
                file.close();
                details = QString("Downloaded to '%1'...").arg(filePath);
            } else {
                status = ResultStatus::FAILED;
                details = file.errorString();
            }
        } else {
            std::string html = m_tidy->process(response.text);
            m_dom->from_string(html);
            QStringList texts;
            for (const auto& el : m_dom->select_all("//body//*")) {
                if (el.name() != "script" and el.name() != "style") {
                    QString text(strip(el.text()).c_str());
                    if (text.length() > 0)
                        texts.append(text);
                }
            }
            const QString fileName(QUrl::toPercentEncoding(url.toString()) + ".txt");
            const QString filePath(downloadDirectory.filePath(fileName));
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                logMessage(QString("Downloading HTML text to '%1'...").arg(filePath));
                QTextStream textStream(&file);
                textStream << texts.join(QStringLiteral(" "));
                file.close();
                details = QString("Downloaded to '%1'...").arg(filePath);
            } else {
                status = ResultStatus::FAILED;
                details = file.errorString();
            }
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }
    auto data = QMap<QString, QVariant> {
        { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
        { QStringLiteral("URL"), inputData[FIELD_URL] },
        { QStringLiteral("Details"), QVariant(details) },

        { QStringLiteral("Result"), QVariant(ResultStatusText.value(status)) }
    };

    Q_EMIT Worker::result(m_toolId, data);
    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
