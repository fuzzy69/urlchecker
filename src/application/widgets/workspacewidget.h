#pragma once

//#include <QMutex>
//#include <QQueue>
#include <QWidget>

#include "../core/resultstatus.h"
#include "../tools/tools.h"

class QHBoxLayout;
class QPushButton;
class QProgressBar;
// class QTableView;
class QSplitter;
class QTabWidget;
class QTreeWidget;
class QVBoxLayout;

class FilesystemWidget;
class LogWidget;
class Table;
class TablesWidget;
//class Thread;
class ToolsWidget;
//class Worker;
class WorkerManager;

class Tool;

class WorkspaceWidget final: public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    ToolsWidget* toolsWidget();
    FilesystemWidget* filesystemWidget();
    TablesWidget* tablesWidget();
    LogWidget* logWidget();
    WorkerManager* workerManager();
    void setCurrentProgress(int value);

public Q_SLOTS:
    void toggleTools();
    void toggleSideTabWidget();
    void toggleLogWidget();

    // TODO: managing application states doesn't look too good, find a better approach
    void onApplicationStart();
    void onApplicationReady();
    void onApplicationExit();
    void onJobStart();
    void onJobStop();
    void onJobDone();

Q_SIGNALS:
    void jobStarted();
    void jobStopped();
    void log(const QString& message);
//     void test();

protected:
// //     void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

//    void onResult(const QVariantMap &resultData);
    void onResult(Tools toolId, const QVariantMap &resultData);
    void onStatus(const int rowId, const ResultStatus &resultStatus);
    void onItemDone();

private Q_SLOTS:
    void startJob();
    void stopJob();
    
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_bottomLayout;
    QSplitter *m_splitter;
    QSplitter *m_verticalSplitter;
    QSplitter *m_horizontalSplitter;

    QPushButton *m_startPushButton;
    QPushButton *m_stopPushButton;
//    QPushButton *m_testPushButton;
    QProgressBar *m_progressBar;

    QTabWidget *m_sideTabWidget;
    ToolsWidget *m_toolsWidget;
    FilesystemWidget *m_fileSystemWidget;
    TablesWidget *m_tablesWidget;
    LogWidget *m_logWidget;

    WorkerManager* m_workerManager;
//    QList<Thread*> m_threads;
//    QList<Worker*> m_workers;
//    QQueue<QVariantMap> m_inputDataQueue;
//    QMutex m_mutex;

//    int m_itemsDone;
//    int m_totalItems;
};
