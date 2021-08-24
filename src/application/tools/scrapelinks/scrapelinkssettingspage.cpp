#include "scrapelinkssettingspage.h"
#include "ui_scrapelinkssettings.h"

#include "../../config.h"
#include "../../constants.h"
#include "../../texts.h"
#include "../../core/settings.h"
#include "../../icons.h"

ScrapeLinksSettingsPage::ScrapeLinksSettingsPage(QWidget *parent) : QWidget(parent), m_ui(new Ui_ScrapeLinksSettings)
{
    m_ui->setupUi(this);
}

ScrapeLinksSettingsPage::~ScrapeLinksSettingsPage()
{
    delete m_ui;
}

void ScrapeLinksSettingsPage::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(QStringLiteral(SCRAPE_LINKS_STRATEGY), QVariant(static_cast<int>(scrapeLinksStrategy())));
}

void ScrapeLinksSettingsPage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    setScrapeLinksStrategy(static_cast<ScrapeLinksStrategy>(Settings::instance().value(QStringLiteral(SCRAPE_LINKS_STRATEGY)).toInt()));
}

void ScrapeLinksSettingsPage::setScrapeLinksStrategy(ScrapeLinksStrategy scrapeLinksStrategy)
{
    switch (scrapeLinksStrategy)
    {
        case ScrapeLinksStrategy::ALL_LINKS:
            m_ui->scrapeAllLinksRadioButton->setChecked(true);
            break;
        case ScrapeLinksStrategy::INTERNAL_LINKS:
            m_ui->scrapeInternalLinksRadioButton->setChecked(true);
            break;
        case ScrapeLinksStrategy::EXTERNAL_LINKS:
            m_ui->scrapeExternalLinksRadioButton->setChecked(true);
            break;
    }
}

ScrapeLinksStrategy ScrapeLinksSettingsPage::scrapeLinksStrategy()
{
    if (m_ui->scrapeAllLinksRadioButton->isChecked())
        return ScrapeLinksStrategy::ALL_LINKS;
    else if (m_ui->scrapeInternalLinksRadioButton->isChecked())
        return ScrapeLinksStrategy::INTERNAL_LINKS;
    else
        return ScrapeLinksStrategy::EXTERNAL_LINKS;
}
