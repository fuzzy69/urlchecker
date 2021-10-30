#pragma once

class QProgressBar;
//class QStatusBar;

class LogWidget;
class ProxiesWidget;
class SettingsWidget;
class StatusBarWidget;
class TablesWidget;
class ToolsWidget;
class WorkspaceWidget;

class ApplicationBridge final {
public:
    ~ApplicationBridge() = default;

    static ApplicationBridge& instance();

    TablesWidget* tablesWidget();
    ToolsWidget* toolsWidget();
    LogWidget* logWidget();
    ProxiesWidget* proxiesWidget();
    SettingsWidget* settingsWidget();
    WorkspaceWidget* workspaceWidget();
    QProgressBar* progressBar();
    StatusBarWidget* statusBarWidget();

    void setTablesWidget(TablesWidget* tablesWidget);
    void setToolsWidget(ToolsWidget* toolsWidget);
    void setLogWidget(LogWidget* logWidget);
    void setProxiesWidget(ProxiesWidget* proxiesWidget);
    void setSettingsWidget(SettingsWidget* settingsWidget);
    void setWorkspaceWidget(WorkspaceWidget* workspaceWidget);
    void setProgressBar(QProgressBar* progressBar);
    void setStatusBarWidget(StatusBarWidget* statusBarWidget);

private:
    explicit ApplicationBridge() = default;

    static ApplicationBridge* m_instance;

    TablesWidget* m_tablesWidget = nullptr;
    ToolsWidget* m_toolsWidget = nullptr;
    LogWidget* m_logWidget = nullptr;
    ProxiesWidget* m_proxiesWidget = nullptr;
    SettingsWidget* m_settingsWidget = nullptr;
    WorkspaceWidget* m_workspaceWidget = nullptr;
    QProgressBar* m_progressBar = nullptr;
    StatusBarWidget* m_statusBarWidget = nullptr;
};
