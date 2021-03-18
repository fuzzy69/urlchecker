#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QPushButton>
#include <QProgressBar>
#include <QQueue>
#include <QTableView>
#include <QTabWidget>
#include <QUrl>
#include <QDesktopServices>
#include <QMap>
#include <QVariant>

#include "workspacewidget.h"
#include "../icons.h"
#include "../common/table.h"
#include "../common/thread.h"
#include "../core/toolswidget.h"
#include "../core/tools.h"
#include "../workers/checkurlstatusworker.h"
#include "../workers/checkalexarank.h"
#include "../workers/scrapeproxies.h"
#include "../workers/dummyworker.h"


WorkspaceWidget::WorkspaceWidget(QWidget* parent) : QWidget(parent)
{
    m_threads = QList<Thread*>();
    m_workers = QList<Worker*>();
    m_inputDataQueue = QQueue<QMap<QString, QVariant>>();

    m_mainLayout = new QVBoxLayout(this);
    m_topLayout = new QHBoxLayout;
    m_bottomLayout = new QHBoxLayout;

    m_toolsWidget = new ToolsWidget(Tool(
        Tools::CHECK_URL_STATUS, 
        QIcon(ICON_CHAIN), 
        QString("Check URL Status"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5 << 0.3
    ));
    m_toolsWidget->addTool(Tool(
        Tools::CHECK_ALEXA_RANK, 
        QIcon(ICON_ALEXA), 
        QString("Check Alexa Rank"),
        QStringList() << "URL" << "Rank" << "Status",
        QList<float>() << 0.5 << 0.3
    ));
    m_toolsWidget->addTool(Tool(
        Tools::SCRAPE_PROXIES, 
        QIcon(ICON_MASK), 
        QString("Scrape Proxies"),
        QStringList() << "Proxy" << "Source" << "Status",
        QList<float>() << 0.5 << 0.3
    ));
//     m_toolsWidget->addTool(Tool(Tools::SCRAPE_PROXIES, QIcon(ICON_MASK), QString("Scrape Proxies")));
//     connect(m_toolsWidget, &ToolsWidget::toolSelected, [this](const Tool &tool){
//         m_toolsPushButton->setIcon(tool.icon());
//         m_toolsPushButton->setText(tool.name());
//     });
    m_tabWidget = new QTabWidget;
    m_inputTable = new Table(QStringList() << "URL" << "Status", this);
    m_inputTable->setColumnRatios(QList<float>() << 0.8);
//     m_resultsTable = new Table(QStringList() << "URL" << "Result" << "Code" << "Status", this);
//     m_resultsTable->setColumnRatios(QList<float>() << 0.5 << 0.2 << 0.2);
    m_resultsTable = new Table(m_toolsWidget->currentTool().columns(), this);
    m_resultsTable->setColumnRatios(m_toolsWidget->currentTool().columnRatios());
    m_startPushButton = new QPushButton(QIcon(ICON_CONTROL), "Start");
    m_stopPushButton = new QPushButton(QIcon(ICON_CONTROL_STOP), "Stop");
    m_testPushButton = new QPushButton("Test");
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);

    m_tabWidget->addTab(m_inputTable->tableView(), QString("Source URLs"));
    m_tabWidget->addTab(m_resultsTable->tableView(), QString("Results"));
    m_topLayout->addWidget(m_toolsWidget);
    m_topLayout->addWidget(m_tabWidget);
    m_bottomLayout->addStretch(0);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    m_bottomLayout->addWidget(m_testPushButton);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_bottomLayout);
    m_mainLayout->addWidget(m_progressBar);

    connect(m_inputTable, &Table::doubleClicked, [this] (const QModelIndex &modelIndex) {
        QDesktopServices::openUrl(QUrl(m_inputTable->cell(modelIndex.row(), 0).toString()));
    });
    connect(m_resultsTable, &Table::doubleClicked, [this] (const QModelIndex &modelIndex) {
        QDesktopServices::openUrl(QUrl(m_resultsTable->cell(modelIndex.row(), 0).toString()));
    });
    connect(m_startPushButton, &QPushButton::clicked, this, &WorkspaceWidget::startJob);
    connect(m_stopPushButton, &QPushButton::clicked, this, &WorkspaceWidget::stopJob);
//     connect(m_testPushButton, &QPushButton::clicked, this, &WorkspaceWidget::test);
    connect(m_testPushButton, &QPushButton::clicked, [this]{
        auto x = m_toolsWidget->getTool("Check URL Status");
        qDebug() << x.name();
    });
}

void WorkspaceWidget::toggleTools()
{
    if (m_toolsWidget->isVisible())
        m_toolsWidget->setVisible(false);
    else
        m_toolsWidget->setVisible(true);
}

Table * WorkspaceWidget::inputTable()
{
    return m_inputTable;
}

Table * WorkspaceWidget::resultsTable()
{
    return m_resultsTable;
}

ToolsWidget * WorkspaceWidget::toolsWidget()
{
    return m_toolsWidget;
}

void WorkspaceWidget::setCurrentProgress(int value)
{
    m_progressBar->setValue(value);
}

void WorkspaceWidget::updateResultsRow(const QMap<QString, QVariant>& resultData)
{
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    //     updateResultsRow(resultData["rowId"].toInt(), resultData["result"].toString(), resultData["status"].toInt(), resultData["message"].toString());
    m_resultsTable->appendRow(QStringList() << QString(rowIndex) << result << QString("xxx") << QString("yyy"));
//     m_resultsTable->setCell(rowIndex, 1, QVariant(result));
//     m_resultsTable->setCell(rowIndex, 2, QVariant(result));
// //     m_resultsTable->setCell(rowIndex, 3, QVariant(statusText));
// //     int _statusCode = statusCode.toInt();
// //     if (_statusCode >= 200 && _statusCode < 300)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkGreen));
// //     else if (_statusCode >= 300 && _statusCode < 400)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkBlue));
// //     else if (_statusCode >= 400)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkRed));
// //     else
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::darkGray), QColor(Qt::darkYellow));
}

Table * WorkspaceWidget::focusedTable()
{
    return (m_tabWidget->currentIndex() == 0)? m_inputTable : m_resultsTable;
}

void WorkspaceWidget::invertSelectedRows()
{
    auto currentTable = focusedTable();
    currentTable->invertSelection();
}

void WorkspaceWidget::removeAllRows()
{
    auto currentTable = focusedTable();
    currentTable->removeAllRows();
}

void WorkspaceWidget::removeDuplicatedRows()
{
    auto currentTable = focusedTable();
    currentTable->removeDuplicates();
}

void WorkspaceWidget::removeSelectedRows()
{
    auto currentTable = focusedTable();
    currentTable->removeSelected();
}

void WorkspaceWidget::selectAllRows()
{
    auto currentTable = focusedTable();
    currentTable->selectAll();
}

void WorkspaceWidget::onApplicationStart()
{
    m_startPushButton->setEnabled(false);
    m_stopPushButton->setEnabled(false);
}

void WorkspaceWidget::onApplicationReady()
{
    m_startPushButton->setEnabled(true);
    m_stopPushButton->setEnabled(false);
}

void WorkspaceWidget::onApplicationExit()
{
    m_startPushButton->setEnabled(false);
    m_stopPushButton->setEnabled(false);
}

void WorkspaceWidget::onJobStart()
{
    m_startPushButton->setEnabled(false);
    m_stopPushButton->setEnabled(true);
}

void WorkspaceWidget::onJobStop()
{
    m_startPushButton->setEnabled(false);
    m_stopPushButton->setEnabled(false);
}

void WorkspaceWidget::onJobDone()
{
    m_startPushButton->setEnabled(false);
    m_stopPushButton->setEnabled(false);
}

void WorkspaceWidget::resizeEvent(QResizeEvent* event)
{
    focusedTable()->resizeColumns();
    QWidget::resizeEvent(event);
}

void WorkspaceWidget::clearResultsTable()
{
//     m_resultsTable->removeAllRows();
    // TODO: Improve table creation, eg. skip creating new table if tool is not changed
    m_resultsTable = new Table(m_toolsWidget->currentTool().columns(), this);
    m_resultsTable->setColumnRatios(m_toolsWidget->currentTool().columnRatios());
}

void WorkspaceWidget::startJob()
{
    auto currentTool = toolsWidget()->currentTool();
    m_resultsTable->resetColumns(currentTool.columns());
    m_resultsTable->setColumnRatios(currentTool.columnRatios());
//     m_resultsTable = new Table(currentTool.columns(), this);
//     m_resultsTable->setColumnRatios(currentTool.columnRatios());
//     clearResultsTable();
    m_itemsDone = 0;
    m_totalItems = m_inputTable->rowCount();
    m_threads.clear();
    m_workers.clear();
    for (int i = 0; i < m_inputTable->rowCount(); ++i)
    {
        auto url = m_inputTable->cell(i, 0).toString();
        m_inputDataQueue.enqueue({
            {QString("rowId"), QVariant(i)},
            {QString("url"), QVariant(url)}
        });
    }
    int parallelTasks = 1;
//     int parallelTasks = m_settingsWidget->threadCount();
    for (int i = 0; i < parallelTasks;++i)
    {
        auto thread = new Thread;
        Worker *worker;

        // TODO: Improve tool switching logic
        switch (currentTool.id())
        {
            case Tools::CHECK_URL_STATUS:
                worker = new CheckUrlStatusWorker(m_inputDataQueue);
                break;
            case Tools::CHECK_ALEXA_RANK:
                worker = new CheckAlexaRankWorker(m_inputDataQueue);
                break;
            case Tools::SCRAPE_PROXIES:
                worker = new ScrapeProxiesWorker(m_inputDataQueue);
                break;
            default:
                worker = new DummyWorker(m_inputDataQueue);
        }

//         worker = new DummyWorker(m_inputDataQueue);
        m_threads.append(thread);
        m_workers.append(worker);
        m_workers[i]->moveToThread(m_threads[i]);

        // Connections
        connect(thread, &Thread::started, worker, &Worker::run);
        connect(thread, &Thread::finished, thread, &Thread::deleteLater);
        connect(worker, &Worker::result, this, &WorkspaceWidget::onResult);
        connect(worker, &Worker::finished, thread, &Thread::quit);
        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
        connect(worker, &Worker::finished, []{
            qDebug() << "Worker finished";
        });
        connect(worker, &Worker::requestStop, worker, &Worker::stop);
    }

//     emit m_applicationStateMachine->jobStart();
    for (int i = 0; i < parallelTasks; ++i)
    {
        m_threads[i]->start();
    }
}

void WorkspaceWidget::stopJob()
{
    for (Worker *worker: m_workers)
    {
        if (worker)
        {
            emit worker->requestStop();
        }
    }
//     emit m_applicationStateMachine->jobStop();
}

void WorkspaceWidget::onResult(const QMap<QString, QVariant>& resultData)
{
    qDebug() << "on result";
    ++m_itemsDone;
//     Tools currentToolId = static_cast<Tools>(resultData["toolId"].toInt());
    auto currentToolName = resultData["toolName"].toString();
    qDebug() << currentToolName;
    Tool currentTool = m_toolsWidget->getTool(currentToolName);
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    QStringList row;
    qDebug() << currentToolName;
    qDebug() << currentTool.columns();
    for (const auto& column : currentTool.columns())
    {
        row << resultData[column].toString();
        qDebug() << resultData[column].toString();
    }
    qDebug() << row;
    m_resultsTable->appendRow(row);
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    m_progressBar->setValue(progresPercentage);
}
