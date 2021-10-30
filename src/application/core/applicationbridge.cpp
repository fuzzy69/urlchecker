#include "applicationbridge.h"

#include "../widgets/statusbarwidget.h"

ApplicationBridge& ApplicationBridge::instance()
{
    static ApplicationBridge instance;

    return instance;
}

TablesWidget* ApplicationBridge::tablesWidget()
{
    return m_tablesWidget;
}

ToolsWidget* ApplicationBridge::toolsWidget()
{
    return m_toolsWidget;
}

LogWidget* ApplicationBridge::logWidget()
{
    return m_logWidget;
}

ProxiesWidget* ApplicationBridge::proxiesWidget()
{
    return m_proxiesWidget;
}

SettingsWidget* ApplicationBridge::settingsWidget()
{
    return m_settingsWidget;
}

WorkspaceWidget* ApplicationBridge::workspaceWidget()
{
    return m_workspaceWidget;
}

QProgressBar* ApplicationBridge::progressBar()
{
    return m_progressBar;
}

StatusBarWidget* ApplicationBridge::statusBarWidget()
{
    return m_statusBarWidget;
}

void ApplicationBridge::setTablesWidget(TablesWidget* tablesWidget)
{
    m_tablesWidget = tablesWidget;
}

void ApplicationBridge::setToolsWidget(ToolsWidget* toolsWidget)
{
    m_toolsWidget = toolsWidget;
}

void ApplicationBridge::setLogWidget(LogWidget* logWidget)
{
    m_logWidget = logWidget;
}

void ApplicationBridge::setProxiesWidget(ProxiesWidget* proxiesWidget)
{
    m_proxiesWidget = proxiesWidget;
}

void ApplicationBridge::setSettingsWidget(SettingsWidget* settingsWidget)
{
    m_settingsWidget = settingsWidget;
}

void ApplicationBridge::setWorkspaceWidget(WorkspaceWidget* workspaceWidget)
{
    m_workspaceWidget = workspaceWidget;
}

void ApplicationBridge::setProgressBar(QProgressBar* progressBar)
{
    m_progressBar = progressBar;
}

void ApplicationBridge::setStatusBarWidget(StatusBarWidget* statusBarWidget)
{
    m_statusBarWidget = statusBarWidget;
}
