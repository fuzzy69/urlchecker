#include "connectionsettingspage.h"
#include "ui_connectionsettings.h"

#include "../../config.h"
#include "../../texts.h"
#include "../../core/settings.h"
#include "../../icons.h"

ConnectionSettingsPage::ConnectionSettingsPage(QWidget *parent) : QWidget(parent), m_ui(new Ui_ConnectionSettings)
{
    m_ui->setupUi(this);
}

ConnectionSettingsPage::~ConnectionSettingsPage()
{
    delete m_ui;
}

void ConnectionSettingsPage::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(TEXT_RETRIES, QVariant(m_ui->connectionRetriesSpinBox->value()));
    Settings::instance().setValue(TEXT_TIMEOUT, QVariant(m_ui->connectionTimeoutSpinBox->value()));
    Settings::instance().setValue(TEXT_VERIFY_SSL, QVariant(m_ui->verifySslCheckBox->isChecked()));
}

void ConnectionSettingsPage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    m_ui->connectionRetriesSpinBox->setValue(Settings::instance().value(QStringLiteral(TEXT_RETRIES)).toInt());
    m_ui->connectionTimeoutSpinBox->setValue(Settings::instance().value(QStringLiteral(TEXT_TIMEOUT)).toInt());
    m_ui->verifySslCheckBox->setChecked(Settings::instance().value(TEXT_VERIFY_SSL).toBool());
}
