#pragma once

#include <QMainWindow>
#include <QQueue>
#include <QPair>
#include <QUrl>
#include <QList>

#include "../common/basemainwindow.h"
#include "../utils/useragents.h"

// class QAction;
class QHBoxLayout;
class QLabel;
// class QMenu;
// class QProgressBar;
class QPushButton;
// class QSpinBox;
class QStackedWidget;
// class QStandardItemModel;
// class QStatusBar;
// class QTableView;
class QTimer;
// class QToolBar;
// class QVBoxLayout;
// class QWidget;
// class QTreeWidget;
// class QTabWidget;
// 
class ApplicationStateMachine;
class HelpWidget;
class RecentFiles;
class ProxiesWidget;
class SideBar;
// class Table;
// class ToolsWidget;
class SettingsWidget;
class Thread;
class Worker;
// class UserAgents;
class WorkspaceWidget;


class MainWindow : public BaseMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void workerStop();

protected slots:
    void importUrls();
    void exportResults();
    void importRecentFileUrls(const QString &filePath);
//     void startJob();
//     void stopJob();
//     void onResult(const QMap<QString, QVariant> &resultData);

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

//     QVBoxLayout *m_projectPageLayout;
//     QVBoxLayout *m_settingsPageLayout;
//     QVBoxLayout *m_proxiesPageLayout;
//     QVBoxLayout *m_helpPageLayout;
// 
//     QVBoxLayout *m_mainLayout;
//     QHBoxLayout *m_bottomLayout;
//     Table *m_inputTable;
//     Table *m_resultsTable;
//     QList<float> m_columnRatios;
//     QLabel *m_threadsLabel;
//     QLabel *m_timeoutLabel;
//     QSpinBox *m_threadsSpinBox;
//     QSpinBox *m_timeoutSpinBox;
//     QPushButton *m_startPushButton;
//     QPushButton *m_stopPushButton;
//     QPushButton *m_testPushButton;
//     QProgressBar *m_progressBar;

    // Statusbar
    QStatusBar *m_statusBar;
    QPushButton *m_toolsPushButton;
    QLabel *m_activeThreadsLabel;
    QLabel *m_statusBarLabel;

// 
//     int m_currentRowIndex = 0;
//     bool m_running = false;
//     QList<QAction*> m_recentUrlFileActions;
// 
//     QStandardItemModel *m_resultsModel;

    QTimer *m_pulseTimer;
    RecentFiles *m_recentFiles;

//     QList<Thread*> m_threads;
//     QList<Worker*> m_workers;
//     QQueue<QMap<QString, QVariant>> m_inputDataQueue;
    ApplicationStateMachine *m_applicationStateMachine;

//     ToolsWidget *m_toolsWidget;
// 


//     UserAgents m_userAgents;

    QString m_settingsFilePath;
    QString m_lastDirectory;

//     int m_itemsDone;
//     int m_totalItems;
};
