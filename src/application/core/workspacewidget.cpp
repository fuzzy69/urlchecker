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
#include "../config.h"
#include "../icons.h"
#include "../common/settings.h"
#include "../common/table.h"
#include "../common/thread.h"
#include "../core/toolswidget.h"
#include "../core/tools.h"
#include "../workers/checkurlstatusworker.h"
#include "../workers/checkalexarank.h"
#include "../workers/scrapeproxies.h"
#include "../workers/dummyworker.h"
#include "../workers/resultstatus.h"
#include "../workers/testworker.h"

#include "../libs/cpr/include/cpr/cpr.h"


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
    m_toolsWidget->addTool(Tool(
        Tools::TEST, 
        QIcon(ICON_HAMMER), 
        QString("Test"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5 << 0.3
    ));
#if APP_DEBUG
    m_toolsWidget->addTool(Tool(
        Tools::DUMMY, 
        QIcon(ICON_QUESTION), 
        QString("Dummy"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5 << 0.3
    ));
#endif
    m_tabWidget = new QTabWidget;
    m_inputTable = new Table(QStringList() << "URL" << "Status", this);
    m_inputTable->setColumnRatios(QList<float>() << 0.8);
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
    connect(m_testPushButton, &QPushButton::clicked, [this]{
        qDebug() << "Test";
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

void WorkspaceWidget::updateResultsRow(const QVariantMap& resultData)
{
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    m_resultsTable->appendRow(QStringList() << QString(rowIndex) << result << QString("xxx") << QString("yyy"));
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
    // TODO: Improve table creation, eg. skip creating new table if tool is not changed
    m_resultsTable = new Table(m_toolsWidget->currentTool().columns(), this);
    m_resultsTable->setColumnRatios(m_toolsWidget->currentTool().columnRatios());
}

void WorkspaceWidget::startJob()
{
    auto currentTool = toolsWidget()->currentTool();
    m_resultsTable->resetColumns(currentTool.columns());
    m_resultsTable->setColumnRatios(currentTool.columnRatios());
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
    const int parallelTasks = Settings::instance().value("parallelTasks").toInt();
    // assert(parallelTasks > 0);
    for (int i = 0; i < parallelTasks;++i)
    {
        auto thread = new Thread;
        Worker *worker;

        // TODO: Improve tool switching logic
        QVariantMap settings;
        settings.insert(QString("timeout"), Settings::instance().value("timeout"));
        settings.insert(QString("useProxies"), Settings::instance().value("useProxies"));
        switch (currentTool.id())
        {
            case Tools::CHECK_URL_STATUS:
                worker = new CheckUrlStatusWorker(m_inputDataQueue, settings);
                break;
            case Tools::CHECK_ALEXA_RANK:
                worker = new CheckAlexaRankWorker(m_inputDataQueue, settings);
                break;
            case Tools::SCRAPE_PROXIES:
                worker = new ScrapeProxiesWorker(m_inputDataQueue, settings);
                break;
            case Tools::TEST:
                worker = new TestWorker(m_inputDataQueue, settings);
                break;
            default:
                worker = new DummyWorker(m_inputDataQueue, settings);
        }
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

    emit jobStarted();
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
    emit jobStopped();
}

void WorkspaceWidget::onResult(const QVariantMap& resultData)
{
    ++m_itemsDone;
//     Tools currentToolId = static_cast<Tools>(resultData["toolId"].toInt());
    auto currentToolName = resultData["toolName"].toString();
    Tool currentTool = m_toolsWidget->getTool(currentToolName);
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    QStringList row;
    for (const auto& column : currentTool.columns())
    {
        if (column == QString("Status"))
        {
            ResultStatus status = static_cast<ResultStatus>(resultData["Status"].toInt());
            switch (status)
            {
                case ResultStatus::OK:
                    row << "Done";
                    break;
                case ResultStatus::PROCESSING:
                    row << "...";
                    break;
                case ResultStatus::FAILED:
                    row << "Failed";
                    break;
            }
        }
        else
        {
        row << resultData[column].toString();
        }
    }
    m_resultsTable->appendRow(row);
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    m_progressBar->setValue(progresPercentage);
}
