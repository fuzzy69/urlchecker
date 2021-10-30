﻿#pragma once

#include <QDir>

#include "widgets/mainwindowbase.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QStackedWidget;

//class ApplicationStateMachine;
class HelpWidget;
class ProxiesWidget;
class RecentFiles;
class SettingsWidget;
class SideBarWidget;
class StatusBarWidget;
class UserAgentsWidget;
class WorkspaceWidget;

class MainWindow final : public MainWindowBase {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

protected Q_SLOTS:
    void onPulse();

private Q_SLOTS:
    void importUrls();
    void importRecentFileUrls(const QString& filePath);
    void exportResults();

private:
    void createMenuBar();
    void createToolBar();
    void createWidgets();
    void createStatusBar();
    void createConnections();

    void loadSettings() override;
    void saveSettings() override;

    void importUrlFile(const QString& filePath);

    void initSettings(const QDir& applicationDir);
    void initProxies(const QDir& applicationDir);
    void initUserAgents(const QDir& applicationDir);

    // MenuBar
    QMenu* m_fileMenu;
    QMenu* m_recentUrlFilesMenu;
    QMenu* m_selectionMenu;
    QMenu* m_editMenu;
    QMenu* m_filterMenu;
    QMenu* m_urlMenu;
    QMenu* m_windowMenu;
    QMenu* m_helpMenu;

    // ToolBar
    QToolBar* m_toolBar;

    // Widgets
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    QVBoxLayout* m_centralLayout;

    SideBarWidget* m_sideBarWidget;

    QStackedWidget* m_mainStackedWidget;

    WorkspaceWidget* m_workspaceWidget;
    SettingsWidget* m_settingsWidget;
    UserAgentsWidget* m_userAgentsWidget;
    ProxiesWidget* m_proxiesWidget;
    HelpWidget* m_helpWidget;

    // Statusbar
    //    QStatusBar* m_statusBar;
    //    QPushButton* m_toolsPushButton;
    //    QPushButton* m_logPushButton;
    //    QLabel* m_activeThreadsLabel;
    //    QLabel* m_statusBarLabel;
    //    QLabel* m_jobStatsLabel;
    //    QLabel* m_jobRuntimeLabel;
    StatusBarWidget* m_statusBarWidget;

    //    ApplicationStateMachine* m_applicationStateMachine;
    QTimer* m_pulseTimer;
    RecentFiles* m_recentFiles;

    // Dirs
    QString m_lastDirectory;

    // Files
    QString m_settingsFilePath;
};
