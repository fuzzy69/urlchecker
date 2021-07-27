#include "applicationbridge.h"

ApplicationBridge& ApplicationBridge::instance()
{
    static ApplicationBridge instance;

    return instance;
}

TablesWidget *ApplicationBridge::tablesWidget()
{
    return m_tablesWidget;
}

ToolsWidget *ApplicationBridge::toolsWidget()
{
    return m_toolsWidget;
}

LogWidget *ApplicationBridge::logWidget()
{
    return m_logWidget;
}

ProxiesWidget *ApplicationBridge::proxiesWidget()
{
    return m_proxiesWidget;
}

SettingsWidget *ApplicationBridge::settingsWidget()
{
    return m_settingsWidget;
}

QProgressBar *ApplicationBridge::progressBar()
{
    return m_progressBar;
}

QStatusBar *ApplicationBridge::statusBar()
{
    return m_statusBar;
}

void ApplicationBridge::setTablesWidget(TablesWidget *tablesWidget)
{
    m_tablesWidget= tablesWidget;
}

void ApplicationBridge::setToolsWidget(ToolsWidget *toolsWidget)
{
    m_toolsWidget = toolsWidget;
}

void ApplicationBridge::setLogWidget(LogWidget *logWidget)
{
    m_logWidget = logWidget;
}

void ApplicationBridge::setProxiesWidget(ProxiesWidget *proxiesWidget)
{
    m_proxiesWidget = proxiesWidget;
}

void ApplicationBridge::setSettingsWidget(SettingsWidget *settingsWidget)
{
    m_settingsWidget = settingsWidget;
}

void ApplicationBridge::setProgressBar(QProgressBar *progressBar)
{
    m_progressBar = progressBar;
}

void ApplicationBridge::setStatusBar(QStatusBar *statusBar)
{
    m_statusBar = statusBar;
}
