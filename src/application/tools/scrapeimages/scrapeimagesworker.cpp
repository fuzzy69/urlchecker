#include <optional>
#include <string>
#include <unordered_set>
#include <qglobal.h>
#ifdef Q_OS_WIN
#include <ciso646>
#endif
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QUrl>

#include "my/text.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/resultstatus.h"
#include "../../utils/requests.h"
#include "../../utils/simpledom.h"
#include "../../utils/tidyhtml.h"
#include "../tools.h"
#include "common.h"
#include "scrapeimagesworker.h"
#include "utilities.h"

using my::text::starts_with;

ScrapeImagesWorker::ScrapeImagesWorker(int id, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings, QObject* parent)
    : Worker(id, inputDataQueue, mutex, settings, parent)
    , m_dom(std::make_unique<SimpleDOM>())
    , m_tidy(std::make_unique<TidyHtml>())
{
    m_toolId = Tools::SCRAPE_IMAGES;
}

ScrapeImagesWorker::~ScrapeImagesWorker()
{
}

void ScrapeImagesWorker::doWork(const QVariantMap& inputData)
{
    int rowId = inputData[FIELD_ROW_ID].toInt();
    QUrl url(inputData[FIELD_URL].toString());
    const QDir downloadImagesDirectory(m_settings[SCRAPE_IMAGES_DIRECTORY].toString());
    const bool downloadImages(m_settings[DOWNLOAD_IMAGES].toBool());
    logMessage(QString("Scraping images from: '%1'...").arg(url.toString()));
    Q_EMIT Worker::status(rowId, ResultStatus::PROCESSING);
    Requests requests(m_settings);
    cpr::Response response = requests.get(url.toString().toStdString());
    std::string html = m_tidy->process(response.text);
    m_dom->from_string(html);
    ResultStatus status((response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED);
    QString details(QStringLiteral("OK"));
    if (status == ResultStatus::OK) {
        std::unordered_set<std::string> images;
        for (auto& element : m_dom->select_all("//img[@src]")) {
            QApplication::processEvents();
            if (!m_running)
                break;
            std::string image = element.attribute("src");
            if (!starts_with(image, "http") or images.count(image) > 0)
                continue;
            // Download image
            if (downloadImages) {
                QUrl imageUrl(image.c_str());
                logMessage(QString("Downloading image '%1'...").arg(imageUrl.toString()));
                if (!imageUrl.fileName().isEmpty()) {
                    const QString imageFilePath = downloadImagesDirectory.filePath(imageUrl.fileName());
                    response = requests.download(image, imageFilePath.toStdString());
                    status = (response.status_code == 200) ? ResultStatus::OK : ResultStatus::FAILED;
                    if (status == ResultStatus::OK) {
                        details = QString("Downloaded to '%1'...").arg(imageFilePath);
                    } else {
                        details = QString::fromUtf8(response.status_line.c_str());
                    }
                }
            }
            images.insert(image);
            auto data = QMap<QString, QVariant> {
                { QStringLiteral(FIELD_ROW_ID), inputData[FIELD_ROW_ID] },
                { QStringLiteral("Source"), inputData[FIELD_URL] },
                { QStringLiteral("Details"), QVariant(details) },

                { QStringLiteral("Image Source"), QVariant(QString::fromUtf8(image.c_str())) }
            };
            Q_EMIT Worker::result(m_toolId, data);
        }
    } else {
        details = QString::fromUtf8(response.status_line.c_str());
    }

    Q_EMIT Worker::itemDone(status == ResultStatus::OK);
    Q_EMIT Worker::status(rowId, status);
}
