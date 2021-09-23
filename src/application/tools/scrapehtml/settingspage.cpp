#include <QFileDialog>
#include <QPushButton>

#include "common.h"
#include "settingspage.h"
#include "ui_settings.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

ScrapeHtmlSettingsPage::ScrapeHtmlSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_settingsPageWidget)
{
    m_ui->setupUi(this);
    connect(m_ui->browsePushButton, &QPushButton::clicked, [this] {
        const QString dirPath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        QDir dir(dirPath);
        if (dir.isReadable())
            m_ui->downloadHtmlDirectoryLineEdit->setText(dirPath);
    });
}

ScrapeHtmlSettingsPage::~ScrapeHtmlSettingsPage()
{
    delete m_ui;
}

void ScrapeHtmlSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(QStringLiteral(HTML_DIRECTORY), QVariant(m_ui->downloadHtmlDirectoryLineEdit->text()));
    Settings::instance().setValue(QStringLiteral(SCRAPE_HTML_STRATEGY), QVariant(static_cast<int>(scrapeHtmlStrategy())));
}

void ScrapeHtmlSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    const auto htmlDirectory = Settings::instance().value(QStringLiteral(HTML_DIRECTORY)).toString();
    m_ui->downloadHtmlDirectoryLineEdit->setText(htmlDirectory);
    setScrapeHtmlStrategy(static_cast<ScrapeHtmlStrategy>(Settings::instance().value(QStringLiteral(SCRAPE_HTML_STRATEGY)).toInt()));
}

void ScrapeHtmlSettingsPage::setScrapeHtmlStrategy(ScrapeHtmlStrategy strategy)
{
    switch (strategy) {
    case ScrapeHtmlStrategy::FULL_HTML:
        m_ui->scrapeHtmlRadioButton->setChecked(true);
        break;
    case ScrapeHtmlStrategy::VISIBLE_TEXT:
        m_ui->scrapeTextRadioButton->setChecked(true);
        break;
    }
}

ScrapeHtmlStrategy ScrapeHtmlSettingsPage::scrapeHtmlStrategy()
{
    if (m_ui->scrapeHtmlRadioButton->isChecked())
        return ScrapeHtmlStrategy::FULL_HTML;
    else
        return ScrapeHtmlStrategy::VISIBLE_TEXT;
}
