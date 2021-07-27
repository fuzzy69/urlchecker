#pragma once

//#include <QObject>

class QProgressBar;
class QStatusBar;

class LogWidget;
class ProxiesWidget;
class SettingsWidget;
class TablesWidget;
class ToolsWidget;

class ApplicationBridge final
{
public:
    ~ApplicationBridge() = default;

    static ApplicationBridge& instance();

    TablesWidget* tablesWidget();
    ToolsWidget* toolsWidget();
    LogWidget* logWidget();
    ProxiesWidget* proxiesWidget();
    SettingsWidget* settingsWidget();
    QProgressBar* progressBar();
    QStatusBar* statusBar();

    void setTablesWidget(TablesWidget* tablesWidget);
    void setToolsWidget(ToolsWidget* toolsWidget);
    void setLogWidget(LogWidget* logWidget);
    void setProxiesWidget(ProxiesWidget* proxiesWidget);
    void setSettingsWidget(SettingsWidget* settingsWidget);
    void setProgressBar(QProgressBar* progressBar);
    void setStatusBar(QStatusBar* statusBar);

private:
    explicit ApplicationBridge() = default;

    static ApplicationBridge *m_instance;

    TablesWidget* m_tablesWidget = nullptr;
    ToolsWidget* m_toolsWidget = nullptr;
    LogWidget* m_logWidget = nullptr;
    ProxiesWidget* m_proxiesWidget = nullptr;
    SettingsWidget* m_settingsWidget = nullptr;
    QProgressBar* m_progressBar = nullptr;
    QStatusBar* m_statusBar= nullptr;
};
