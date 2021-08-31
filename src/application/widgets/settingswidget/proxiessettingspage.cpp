#include "proxiessettingspage.h"
#include "ui_proxiessettings.h"

#include "../../config.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

ProxiesSettingsPage::ProxiesSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_ProxiesSettings)
{
    m_ui->setupUi(this);
}

ProxiesSettingsPage::~ProxiesSettingsPage()
{
    delete m_ui;
}

void ProxiesSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(TEXT_USE_PROXIES, QVariant(m_ui->useProxiesCheckBox->isChecked()));
}

void ProxiesSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    m_ui->useProxiesCheckBox->setChecked(Settings::instance().value(QStringLiteral(TEXT_USE_PROXIES)).toBool());
}
