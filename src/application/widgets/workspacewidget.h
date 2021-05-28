#pragma once

#include <QMutex>
#include <QQueue>
#include <QWidget>

#include "../workers/resultstatus.h"

class QHBoxLayout;
class QPushButton;
class QProgressBar;
// class QTableView;
class QSplitter;
class QTabWidget;
class QTreeWidget;
class QVBoxLayout;

class Table;
class TablesWidget;
class Thread;
class ToolsWidget;
class Worker;

class WorkspaceWidget final: public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    ToolsWidget* toolsWidget();
    TablesWidget* tablesWidget();
    void setCurrentProgress(int value);

public Q_SLOTS:
    void toggleTools();

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
//     void test();

protected:
// //     void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void onResult(const QVariantMap &resultData);
    void onStatus(const qint8 rowId, const ResultStatus &resultStatus);

private Q_SLOTS:
    void startJob();
    void stopJob();
    
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_bottomLayout;
    QSplitter *m_splitter;

    QPushButton *m_startPushButton;
    QPushButton *m_stopPushButton;
    QPushButton *m_testPushButton;
    QProgressBar *m_progressBar;

    QTabWidget *m_sideTabWidget;
    ToolsWidget *m_toolsWidget;
    QTreeWidget *m_fileSystemWidget;
    TablesWidget *m_tablesWidget;

    QList<Thread*> m_threads;
    QList<Worker*> m_workers;
    QQueue<QVariantMap> m_inputDataQueue;
    QMutex m_mutex;

    int m_itemsDone;
    int m_totalItems;
};
