#include "workspacewidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QProgressBar>
#include <QSplitter>
#include <QTableView>
#include <QTabWidget>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>

#include "toolswidget.h"
#include "../icons.h"
#include "../texts.h"
#include "../core/applicationbridge.h"
#include "../core/settings.h"
#include "../core/table.h"
//#include "../core/thread.h"
#include "../core/workermanager.h"
#include "../widgets/filesystemwidget.h"
#include "../widgets/logwidget.h"
#include "../widgets/tableswidget.h"

//#include "../tools/workerfactory.h"
//#include "../core/worker.h"
#include "../core/toolsmanager.h"

WorkspaceWidget::WorkspaceWidget(QWidget* parent) : 
    QWidget(parent)/*, m_threads(QList<Thread*>()), m_workers(QList<Worker*>()), m_inputDataQueue(QQueue<QMap<QString, QVariant>>())*/
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    m_topLayout = new QHBoxLayout;
    m_bottomLayout = new QHBoxLayout;
    m_horizontalSplitter = new QSplitter(Qt::Horizontal);
    m_verticalSplitter = new QSplitter(Qt::Vertical);

    m_toolsWidget = new ToolsWidget;
    m_sideTabWidget = new QTabWidget;
    m_tablesWidget = new TablesWidget;
    m_fileSystemWidget = new FilesystemWidget;
    m_logWidget = new LogWidget;
    m_startPushButton = new QPushButton(QIcon(ICON_CONTROL), tr("Start"));
    m_stopPushButton = new QPushButton(QIcon(ICON_CONTROL_STOP), tr("Stop"));
    m_testPushButton = new QPushButton(tr("Test"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_sideTabWidget->addTab(m_toolsWidget, QIcon(ICON_HAMMER), tr("Tools"));
    m_sideTabWidget->addTab(m_fileSystemWidget, QIcon(ICON_DRIVE), tr("Filesystem"));
    m_horizontalSplitter->addWidget(m_sideTabWidget);
    m_horizontalSplitter->addWidget(m_tablesWidget);
    m_horizontalSplitter->setSizes(QList<int>({300, 1000}));
    m_verticalSplitter->addWidget(m_horizontalSplitter);
    m_verticalSplitter->addWidget(m_logWidget);
    m_verticalSplitter->setSizes(QList<int>({1000, 200}));
    m_topLayout->addWidget(m_verticalSplitter);
    m_bottomLayout->addWidget(m_progressBar);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    m_bottomLayout->addWidget(m_testPushButton);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_bottomLayout);

    m_workerManager = new WorkerManager(this);

    ApplicationBridge::instance().setToolsWidget(m_toolsWidget);
    ApplicationBridge::instance().setTablesWidget(m_tablesWidget);
    ApplicationBridge::instance().setLogWidget(m_logWidget);
    ApplicationBridge::instance().setProgressBar(m_progressBar);

    connect(m_startPushButton, &QPushButton::clicked, this, &WorkspaceWidget::startJob);
    connect(m_stopPushButton, &QPushButton::clicked, this, &WorkspaceWidget::stopJob);
    connect(m_workerManager, &WorkerManager::result, this, &WorkspaceWidget::onResult);
    connect(m_workerManager, &WorkerManager::status, this, &WorkspaceWidget::onStatus);
//    connect(m_testPushButton, &QPushButton::clicked, [this] {
//        qDebug() << "Test";
//    });
}

void WorkspaceWidget::toggleTools()
{
    if (m_toolsWidget->isVisible())
        m_toolsWidget->setVisible(false);
    else
        m_toolsWidget->setVisible(true);
}

void WorkspaceWidget::toggleSideTabWidget()
{
    m_sideTabWidget->setVisible(!m_sideTabWidget->isVisible());
}

void WorkspaceWidget::toggleLogWidget()
{
    m_logWidget->setVisible(!m_logWidget->isVisible());
}

ToolsWidget * WorkspaceWidget::toolsWidget()
{
    return m_toolsWidget;
}

FilesystemWidget *WorkspaceWidget::filesystemWidget()
{
    return m_fileSystemWidget;
}

TablesWidget * WorkspaceWidget::tablesWidget()
{
    return m_tablesWidget;
}

LogWidget *WorkspaceWidget::logWidget()
{
    return m_logWidget;
}

void WorkspaceWidget::setCurrentProgress(int value)
{
    m_progressBar->setValue(value);
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
    m_tablesWidget->focusedTable()->resizeColumns();
    QWidget::resizeEvent(event);
}

void WorkspaceWidget::startJob()
{
    if (m_tablesWidget->inputTable()->rowCount() == 0)
    {
        QMessageBox::warning(this, QStringLiteral("Start Job"), QStringLiteral("Nothing to do. Source URLs table is empty!"));
        return;
    }
    qRegisterMetaType<ResultStatus>();
//    auto currentTool = toolsWidget()->currentTool();
    auto currentTool = ToolsManager::instance().currentTool();
    m_tablesWidget->resultsTable()->resetColumns(currentTool.columns());
    m_tablesWidget->resultsTable()->setColumnRatios(currentTool.columnRatios());
    m_workerManager->startJob();
//    m_itemsDone = 0;
//    m_totalItems = m_tablesWidget->inputTable()->rowCount();
//    m_threads.clear();
//    m_workers.clear();
//    for (int i = 0; i < m_tablesWidget->inputTable()->rowCount(); ++i)
//    {
//        auto url = m_tablesWidget->inputTable()->cell(i, 0).toString();
//        m_inputDataQueue.enqueue({
//            {QString("rowId"), QVariant(i)},
//            {QString("url"), QVariant(url)}
//        });
//    }
//    const int parallelTasks = Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt();
//    assert(parallelTasks > 0);
//    for (int i = 0; i < parallelTasks;++i)
//    {
//        int workerId(i+ 1);
////        workerId = i + 1;
//        auto thread = new Thread;
//        Worker *worker;

//        // TODO: Improve tool switching logic
//        QVariantMap settings;
//        settings.insert(QString("timeout"), Settings::instance().value(QStringLiteral(TEXT_TIMEOUT)));
//        settings.insert(QString("useProxies"), Settings::instance().value(QStringLiteral(TEXT_USE_PROXIES)));
//        worker = workerFactory(currentTool.id(), workerId, &m_inputDataQueue, &m_mutex, settings);

//        m_threads.append(thread);
//        m_workers.append(worker);
//        m_workers[i]->moveToThread(m_threads[i]);

//        connect(thread, &Thread::started, worker, &Worker::run);
//        connect(thread, &Thread::finished, thread, &Thread::deleteLater);
//        connect(worker, &Worker::result, this, &WorkspaceWidget::onResult);
//        connect(worker, &Worker::status, this, &WorkspaceWidget::onStatus);
//        connect(worker, &Worker::itemDone, this, &WorkspaceWidget::onItemDone);
//        connect(worker, &Worker::finished, thread, &Thread::quit);
//        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
//        connect(worker, &Worker::finished, []{
//            qDebug() << "Worker finished";
//        });
//        connect(worker, &Worker::requestStop, worker, &Worker::stop);
//        connect(worker, &Worker::log, m_logWidget, &LogWidget::onLog);
//    }

//    Q_EMIT jobStarted();
    m_tablesWidget->switchToResultsTab();
//    for (int i = 0; i < parallelTasks; ++i)
//    {
//        m_threads[i]->start();
//    }
}

void WorkspaceWidget::stopJob()
{
    m_workerManager->stopJob();
}

//    for (Worker *worker: m_workers)
//    {
//        if (worker)
//        {
//            Q_EMIT worker->requestStop();
//        }
//    }
//    Q_EMIT jobStopped();
//}

void WorkspaceWidget::onResult(const QVariantMap& resultData)
{
    if (m_tablesWidget->focusedTable()->rowCount() == 1)
        m_tablesWidget->focusedTable()->resizeColumns();
    auto currentToolName = resultData["toolName"].toString();
    auto currentTool = ToolsManager::instance().getTool(currentToolName);
    QStringList row;
    for (const auto& column : currentTool.columns())
    {
        row << resultData[column].toString();
    }
    m_tablesWidget->resultsTable()->appendRow(row);
}

void WorkspaceWidget::onStatus(const int rowId, const ResultStatus& resultStatus)
{
    QVariant status;
    switch (resultStatus)
    {
        case ResultStatus::OK:
            status = QVariant(QStringLiteral("Done"));
            break;
        case ResultStatus::PROCESSING:
            status = QVariant(QStringLiteral("Processing ..."));
            break;
        case ResultStatus::FAILED:
            status = QVariant(QStringLiteral("Failed"));
            break;
    }
    m_tablesWidget->inputTable()->setCell(rowId, 1, status);
}

//void WorkspaceWidget::onItemDone()
//{
//    ++m_itemsDone;
//    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
//    m_progressBar->setValue(progresPercentage);
//}
