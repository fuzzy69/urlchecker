#include "settingswidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

#include "../config.h"
#include "../texts.h"
#include "../core/settings.h"

SettingsWidget::SettingsWidget ( QWidget* parent ) :
    QWidget(parent)
{
    QDir applicationDir(QApplication::applicationDirPath());
    m_settingsFilePath = applicationDir.absoluteFilePath("settings.ini");

    m_mainLayout = new QVBoxLayout(this);
    m_connectionGroupBox = new QGroupBox("Connection");
    m_proxiesGroupBox = new QGroupBox("Proxies");
    m_threadsLabel = new QLabel("Thread count");
    m_timeoutLabel = new QLabel("Timeout (seconds)");
    m_verifySslLabel = new QLabel("Verify SSL");
    m_verifySslCheckBox = new QCheckBox;
    m_useProxiesLabel = new QLabel("Use proxies");
    m_useProxiesCheckBox = new QCheckBox;
    m_threadsSpinBox = new QSpinBox;
    m_threadsSpinBox->setRange(1, 10);
    m_timeoutSpinBox = new QSpinBox;
    m_timeoutSpinBox->setRange(1, 60);
    m_connectionLayout = new QGridLayout(m_connectionGroupBox);
    m_connectionLayout->addWidget(m_threadsLabel, 0, 0);
    m_connectionLayout->addWidget(m_threadsSpinBox, 0, 1);
    m_connectionLayout->addWidget(m_timeoutLabel, 1, 0);
    m_connectionLayout->addWidget(m_timeoutSpinBox, 1, 1);
    m_connectionLayout->addWidget(m_verifySslLabel, 2, 0);
    m_connectionLayout->addWidget(m_verifySslCheckBox, 2, 1);

    m_proxiesLayout = new QGridLayout(m_proxiesGroupBox);
    m_proxiesLayout->addWidget(m_useProxiesLabel, 0, 0);
    m_proxiesLayout->addWidget(m_useProxiesCheckBox, 0, 1);

    m_mainLayout->addWidget(m_connectionGroupBox);
    m_mainLayout->addWidget(m_proxiesGroupBox);
    m_mainLayout->addStretch(0);
}

void SettingsWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    Settings::instance().setValue( TEXT_THREADS, QVariant(m_threadsSpinBox->value()));
    Settings::instance().setValue( TEXT_TIMEOUT, QVariant(m_timeoutSpinBox->value()));
    Settings::instance().setValue( TEXT_VERIFY_SSL, QVariant(m_verifySslCheckBox->isChecked()));
    Settings::instance().setValue( TEXT_USE_PROXIES, QVariant(m_useProxiesCheckBox->isChecked()));
}

void SettingsWidget::showEvent ( QShowEvent* event )
{
    Q_UNUSED(event)
    m_threadsSpinBox->setValue(Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt());
    m_timeoutSpinBox->setValue(Settings::instance().value( TEXT_TIMEOUT ).toInt());
    m_verifySslCheckBox->setChecked(Settings::instance().value( TEXT_VERIFY_SSL ).toBool());    
    m_useProxiesCheckBox->setChecked(Settings::instance().value( TEXT_USE_PROXIES ).toBool());
}
