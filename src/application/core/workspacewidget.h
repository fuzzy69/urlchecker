#pragma once

#include <QWidget>


class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QProgressBar;
class QTableView;
class QTabWidget;

class Table;
class ToolsWidget;

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    Table* inputTable();
    Table* resultsTable();
    ToolsWidget* toolsWidget();
    void setCurrentProgress(int value);
    void updateResultsRow(const QMap<QString, QVariant>& resultData);

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

    void startJob();
    void stopJob();
    void test();

protected:
//     void hideEvent(QHideEvent *event) override;

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
};
