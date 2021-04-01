#pragma once

#include <QMainWindow>
#include <QQueue>
#include <QPair>
#include <QUrl>
#include <QList>

#include "../common/basemainwindow.h"
#include "../utils/useragents.h"

class QHBoxLayout;
class QLabel;
class QPushButton;
class QStackedWidget;
class QTimer;

class ApplicationStateMachine;
class HelpWidget;
class RecentFiles;
class ProxiesWidget;
class SideBar;
class SettingsWidget;
class Thread;
class Worker;
class WorkspaceWidget;


class MainWindow : public BaseMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void workerStop();

protected slots:
    void importUrls();
    void exportResults();
    void importRecentFileUrls(const QString &filePath);

protected:
//     void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;


private:
    void createActions();
    void createMenuBar();
    void createToolBar();
    void createWidgets();
    void createStatusBar();
    void createConnections();

    void loadSettings() override;
    void saveSettings() override;

    void importUrlFile(const QString &filePath);

    void setStatusMessage(const QString &message);
//     void updateResultsRow(int rowIndex, const QVariant& result, const QVariant &statusCode, const QVariant &statusText);
    void onPulse();
    // Application states
    void onApplicationStart();
    void onApplicationReady();
    void onApplicationExit();
    void onJobStart();
    void onJobStop();
    void onJobDone();

    // Actions
    QAction *m_workspaceAction;
    QAction *m_proxiesAction;
    QAction *m_settingsAction;
    QAction *m_helpAction;
    QAction *m_importUrlsAction;
    QMenu *m_recentUrlFilesMenu;
    QAction *m_clearRecentUrlFilesAction;
    QAction *m_exportResultsAction;
    QAction *m_quitAction;
    QAction *m_removeAllAction;
    QAction *m_removeDuplicatesAction;
    QAction *m_selectAllAction;
    QAction *m_removeSelectedAction;
    QAction *m_invertSelectionAction;
    QAction *m_centerWindowAction;
    QAction *m_aboutAction;

    // MenuBar
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_selectionMenu;
    QMenu *m_windowMenu;
    QMenu *m_helpMenu;

    // ToolBar
    QToolBar *m_toolBar;

    // Widgets
    QWidget *m_centralWidget;
    QHBoxLayout *m_centralLayout;

    SideBar *m_sideBar;

    QStackedWidget *m_mainStackedWidget;
    QWidget *m_workspacePageWidget;
    QWidget *m_settingsPageWidget;
    QWidget *m_proxiesPageWidget;
    QWidget *m_helpPageWidget;

    WorkspaceWidget *m_workspaceWidget;
    SettingsWidget *m_settingsWidget;
    ProxiesWidget *m_proxiesWidget;
    HelpWidget *m_helpWidget;

    // Statusbar
    QStatusBar *m_statusBar;
    QPushButton *m_toolsPushButton;
    QLabel *m_activeThreadsLabel;
    QLabel *m_statusBarLabel;

    QTimer *m_pulseTimer;
    RecentFiles *m_recentFiles;

    ApplicationStateMachine *m_applicationStateMachine;

    QString m_settingsFilePath;
    QString m_lastDirectory;
};
