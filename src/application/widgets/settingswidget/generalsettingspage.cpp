#include "generalsettingspage.h"
#include "ui_generalsettings.h"

#include "../../config.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

GeneralSettingsPage::GeneralSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_GeneralSettings)
{
    m_ui->setupUi(this);
}

GeneralSettingsPage::~GeneralSettingsPage()
{
    delete m_ui;
}

void GeneralSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue(TEXT_THREADS, QVariant(m_ui->workersCountSpinBox->value()));
}

void GeneralSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    m_ui->workersCountSpinBox->setValue(Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt());
}
