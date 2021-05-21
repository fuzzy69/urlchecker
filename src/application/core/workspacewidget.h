#pragma once

#include <QMutex>
#include <QQueue>
#include <QWidget>
#include <QScopedPointer>

#include "../workers/resultstatus.h"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QProgressBar;
class QTableView;
class QTabWidget;

class Table;
class Thread;
class ToolsWidget;
class Worker;

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    Table* inputTable();
    Table* resultsTable();
    ToolsWidget* toolsWidget();
    void setCurrentProgress(int value);
//     void updateResultsRow(const QMap<QString, QVariant>& resultData);
    void updateResultsRow(const QVariantMap& resultData);
    void clearResultsTable();
    void switchToResultsTab();
    void switchToSourcesTab();

public Q_SLOTS:
    void toggleTools();
    void removeAllRows();
    void selectAllRows();
    void invertSelectedRows();
    void removeDuplicatedRows();
    void removeSelectedRows();

    void onApplicationStart();
    void onApplicationReady();
    void onApplicationExit();
    void onJobStart();
    void onJobStop();
    void onJobDone();

Q_SIGNALS:
    void jobStarted();
    void jobStopped();
    void test();

protected:
//     void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void startJob();
    void stopJob();
    void onResult(const QVariantMap &resultData);
    void onStatus(const qint8 rowId, const ResultStatus &resultStatus);

private:
    Table* focusedTable();

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_bottomLayout;

    Table *m_inputTable;
    Table *m_resultsTable;
    QPushButton *m_startPushButton;
    QPushButton *m_stopPushButton;
    QPushButton *m_testPushButton;
    QProgressBar *m_progressBar;
    QTableView *m_tableView;
    QTabWidget *m_tabWidget;
    ToolsWidget *m_toolsWidget;

    QList<Thread*> m_threads;
    QList<Worker*> m_workers;
    QQueue<QVariantMap> m_inputDataQueue;

    int m_itemsDone;
    int m_totalItems;
    // QScopedPointer<QMutex> m_mutex;
    QMutex m_mutex;
};
