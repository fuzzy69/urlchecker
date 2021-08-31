#include <QFileDialog>
#include <QPushButton>

#include "common.h"
#include "scrapeimagessettingspage.h"
#include "ui_scrapeimagessettings.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

ScrapeImagesSettingsPage::ScrapeImagesSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_ScrapeImagesSettings)
{
    m_ui->setupUi(this);
    connect(m_ui->browsePushButton, &QPushButton::clicked, [this] {
        const QString downloadImagesDirectoryPath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        QDir downloadImagesDirectory(downloadImagesDirectoryPath);
        if (!downloadImagesDirectory.isEmpty() and downloadImagesDirectory.isReadable())
            m_ui->downloadImagesDirectoryLineEdit->setText(downloadImagesDirectoryPath);
    });
}

ScrapeImagesSettingsPage::~ScrapeImagesSettingsPage()
{
    delete m_ui;
}

void ScrapeImagesSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(QStringLiteral(SCRAPE_IMAGES_DIRECTORY), QVariant(m_ui->downloadImagesDirectoryLineEdit->text()));
}

void ScrapeImagesSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    const auto scrapeImagesDirectory = Settings::instance().value(QStringLiteral(SCRAPE_IMAGES_DIRECTORY)).toString();
    m_ui->downloadImagesToDirectoryCheckBox->setChecked(!scrapeImagesDirectory.isEmpty());
    m_ui->downloadImagesDirectoryLineEdit->setText(scrapeImagesDirectory);
}
