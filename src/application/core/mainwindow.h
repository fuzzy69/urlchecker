#pragma once

#include <QMainWindow>
#include <QQueue>
#include <QPair>
#include <QUrl>
#include <QList>

class QAction;
class QLabel;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QProgressBar;
class QPushButton;
class QSpinBox;
class QStackedWidget;
class QStandardItemModel;
class QStatusBar;
class QTableView;
class QTimer;
class QToolBar;
class QHBoxLayout;
class QVBoxLayout;
class QWidget;
class QNetworkReply;
class QTreeWidget;

// class HttpClient;
class RecentFiles;
class ProxiesWidget;
class SideBar;
class Table;
class ApplicationState;
class ToolsWidget;
class CheckUrlStatusWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void importUrls();
    void exportResults();
    void removeDuplicates();
    void removeSelected();

//     void startChecking();
//     void stopChecking();
//     void urlChecked(int statusCode, const QString &statusText, const QString &text);
    void onSelectedRecentUrlFile(const QString &filePath);

    void startJob();
    void stopJob();
    void onResult(const QMap<QString, QVariant> &resultData);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void centerWindow();

private:
    void createActions();
    void createMenuBar();
    void createToolBar();
    void createWidgets();
    void createStatusBar();
    void createConnections();
    void loadSettings();
    void saveSettings();
    void initRecentUrlFiles();
    void addToRecentUrlFiles(const QString &filePath);
    void updateResultsRow(int rowIndex, const QVariant &statusCode, const QVariant &statusText);
    void onPulse();

    // Actions
    QAction *m_projectAction;
    QAction *m_proxiesAction;
    QAction *m_settingsAction;
    QAction *m_helpAction;

    QAction *m_importUrlsAction;
    QMenu *m_recentUrlFilesMenu;
    QAction *m_clearRecentUrlFilesAction;
    QAction *m_exportResultsAction;
    QAction *m_quitAction;
    QAction *m_clearTableAction;
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

    QWidget *m_centralWidget;
//     QWidget *m_mainWidget;
    QStackedWidget *m_mainStackedWidget;
    QWidget *m_projectPageWidget;
    QWidget *m_settingsPageWidget;
    QWidget *m_proxiesPageWidget;
    QWidget *m_helpPageWidget;

//     QTextEdit *m_proxiesTextEdit;
    ProxiesWidget *m_proxiesTextEdit;

    QVBoxLayout *m_projectPageLayout;
    QVBoxLayout *m_settingsPageLayout;
    QVBoxLayout *m_proxiesPageLayout;
    QVBoxLayout *m_helpPageLayout;

    QHBoxLayout *m_centralLayout;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_bottomLayout;
//     QTableView *m_resultsTableView;
    Table *m_resultsTable;
    QList<float> m_columnRatios;
    QLabel *m_threadsLabel;
    QLabel *m_timeoutLabel;
    QSpinBox *m_threadsSpinBox;
    QSpinBox *m_timeoutSpinBox;
    QPushButton *m_startPushButton;
    QPushButton *m_stopPushButton;
    QPushButton *m_testPushButton;
    QPushButton *m_toolsPushButton;
    QProgressBar *m_progressBar;
    QStatusBar *m_statusBar;
    QLabel *m_activeThreadsLabel;
    QString m_settingsFilePath;
    QString m_lastDirectory;
//    QQueue<QPair<int, QUrl>> m_dataQueue;
    QList<QNetworkReply*> m_replies;
    QNetworkReply *m_reply;

    QNetworkAccessManager *m_networkManager;
    int m_currentRowIndex = 0;
    bool m_running = false;
    int m_maxRecentFiles = 5;
    QList<QAction*> m_recentUrlFileActions;

    QStandardItemModel *m_resultsModel;
    QTimer *m_pulseTimer;

//     HttpClient *m_httpClient;
    RecentFiles *m_recentFiles;
    SideBar *m_sideBar;
    ApplicationState *m_applicationState;
//     QTreeWidget *m_toolsTreeWidget;
    ToolsWidget *m_toolsWidget;

    QList<QThread*> m_threads;
    QList<CheckUrlStatusWorker*> m_workers;
    QQueue<QMap<QString, QVariant>> m_inputDataQueue;
    int m_itemsDone;
    int m_totalItems;
};
