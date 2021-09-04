#include <QFileDialog>
#include <QPushButton>

#include "common.h"
#include "scrapesitemapssettingspage.h"
#include "ui_scrapesitemapssettings.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

ScrapeSitemapsSettingsPage::ScrapeSitemapsSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_ScrapeSitemapsSettings)
{
    m_ui->setupUi(this);
    connect(m_ui->browsePushButton, &QPushButton::clicked, [this] {
        const QString downloadSitemapsDirectoryPath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        QDir downloadSitemapsDirectory(downloadSitemapsDirectoryPath);
        if (downloadSitemapsDirectory.isReadable())
            m_ui->downloadSitemapsDirectoryLineEdit->setText(downloadSitemapsDirectoryPath);
    });
}

ScrapeSitemapsSettingsPage::~ScrapeSitemapsSettingsPage()
{
    delete m_ui;
}

void ScrapeSitemapsSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(QStringLiteral(SCRAPE_SITEMAPS_DIRECTORY), QVariant(m_ui->downloadSitemapsDirectoryLineEdit->text()));
    Settings::instance().setValue(QStringLiteral(DOWNLOAD_SITEMAPS), QVariant(m_ui->downloadSitemapsToDirectoryCheckBox->isChecked()));
}

void ScrapeSitemapsSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    const auto downloadSitemaps = Settings::instance().value(QStringLiteral(DOWNLOAD_SITEMAPS)).toBool();
    m_ui->downloadSitemapsToDirectoryCheckBox->setChecked(downloadSitemaps);
    const auto scrapeImagesDirectory = Settings::instance().value(QStringLiteral(SCRAPE_SITEMAPS_DIRECTORY)).toString();
    m_ui->downloadSitemapsDirectoryLineEdit->setText(scrapeImagesDirectory);
}
