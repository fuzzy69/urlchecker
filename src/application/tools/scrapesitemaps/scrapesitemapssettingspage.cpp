#include <QPushButton>
#include <QFileDialog>

#include "scrapesitemapssettingspage.h"
#include "ui_scrapesitemapssettings.h"
#include "common.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../texts.h"
#include "../../core/settings.h"
#include "../../icons.h"

ScrapeSitemapsSettingsPage::ScrapeSitemapsSettingsPage(QWidget *parent) : QWidget(parent), m_ui(new Ui_ScrapeSitemapsSettings)
{
    m_ui->setupUi(this);
    connect(m_ui->browsePushButton, &QPushButton::clicked, [this]{
        const QString downloadSitemapsDirectoryPath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        QDir downloadSitemapsDirectory(downloadSitemapsDirectoryPath);
        if (!downloadSitemapsDirectory.isEmpty() and downloadSitemapsDirectory.isReadable())
            m_ui->downloadSitemapsDirectoryLineEdit->setText(downloadSitemapsDirectoryPath);
    });
}

ScrapeSitemapsSettingsPage::~ScrapeSitemapsSettingsPage()
{
    delete m_ui;
}

void ScrapeSitemapsSettingsPage::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(QStringLiteral(SCRAPE_SITEMAPS_DIRECTORY), QVariant(m_ui->downloadSitemapsDirectoryLineEdit->text()));
}

void ScrapeSitemapsSettingsPage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    const auto scrapeImagesDirectory = Settings::instance().value(QStringLiteral(SCRAPE_SITEMAPS_DIRECTORY)).toString();
    m_ui->downloadSitemapsToDirectoryCheckBox->setChecked(!scrapeImagesDirectory.isEmpty());
    m_ui->downloadSitemapsDirectoryLineEdit->setText(scrapeImagesDirectory);
}
