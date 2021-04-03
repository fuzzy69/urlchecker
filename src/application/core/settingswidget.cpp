#include "settingswidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "../common/settings.h"
#include "../config.h"


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

    m_threadsSpinBox->setValue(Settings::instance().value("parallelTasks").toInt());
    m_timeoutSpinBox->setValue(Settings::instance().value("timeout").toInt());
    m_verifySslCheckBox->setChecked(Settings::instance().value("verifySsl").toBool());    
    m_useProxiesCheckBox->setChecked(Settings::instance().value("useProxies").toBool());
//     if (QFile::exists(m_settingsFilePath))
//     {
//         QSettings settings(m_settingsFilePath, QSettings::IniFormat);
//         m_threadsSpinBox->setValue(settings.value("threadsCount", 1).toInt());
//         m_timeoutSpinBox->setValue(settings.value("timeout", 15).toInt());
//         m_useProxiesCheckBox->setChecked(settings.value("useProxies", false).toBool());
//     }
}

void SettingsWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    Settings::instance().setValue("parallelTasks", QVariant(m_threadsSpinBox->value()));
    Settings::instance().setValue("timeout", QVariant(m_timeoutSpinBox->value()));
    Settings::instance().setValue("verifySsl", QVariant(m_verifySslCheckBox->isChecked()));
    Settings::instance().setValue("useProxies", QVariant(m_useProxiesCheckBox->isChecked()));
//     QSettings settings(m_settingsFilePath, QSettings::IniFormat);
//     settings.setValue("threadsCount", m_threadsSpinBox->value());
//     settings.setValue("timeout", m_timeoutSpinBox->value());
//     settings.setValue("useProxies", m_useProxiesCheckBox->isChecked());
}

int SettingsWidget::threadCount() const
{
    return m_threadsSpinBox->value();
}
