#include "workspacewidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
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
#include "../core/settings.h"
#include "../core/table.h"
#include "../core/thread.h"
#include "../widgets/filesystemwidget.h"
#include "../widgets/logwidget.h"
#include "../widgets/tableswidget.h"
#include "../workers/dummyworker.h"
#include "../workers/checkalexarank.h"
#include "../workers/checkurlstatusworker.h"
#include "../workers/scrapeproxies.h"
#include "../workers/testworker.h"
#include "../workers/worker.h"

WorkspaceWidget::WorkspaceWidget(QWidget* parent) : 
    QWidget(parent), m_threads(QList<Thread*>()), m_workers(QList<Worker*>()), m_inputDataQueue(QQueue<QMap<QString, QVariant>>())
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

    connect(m_startPushButton, &QPushButton::clicked, this, &WorkspaceWidget::startJob);
    connect(m_stopPushButton, &QPushButton::clicked, this, &WorkspaceWidget::stopJob);
    connect(m_testPushButton, &QPushButton::clicked, [this] {
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
    qRegisterMetaType<ResultStatus>();
    auto currentTool = toolsWidget()->currentTool();
    m_tablesWidget->resultsTable()->resetColumns(currentTool.columns());
    m_tablesWidget->resultsTable()->setColumnRatios(currentTool.columnRatios());
    m_itemsDone = 0;
    m_totalItems = m_tablesWidget->inputTable()->rowCount();
    m_threads.clear();
    m_workers.clear();
    for (int i = 0; i < m_tablesWidget->inputTable()->rowCount(); ++i)
    {
        auto url = m_tablesWidget->inputTable()->cell(i, 0).toString();
        m_inputDataQueue.enqueue({
            {QString("rowId"), QVariant(i)},
            {QString("url"), QVariant(url)}
        });
    }
    const int parallelTasks = Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt();
    assert(parallelTasks > 0);
    int workerId;
    for (int i = 0; i < parallelTasks;++i)
    {
        workerId = i + 1;
        auto thread = new Thread;
        Worker *worker;

        // TODO: Improve tool switching logic
        QVariantMap settings;
        settings.insert(QString("timeout"), Settings::instance().value(QStringLiteral(TEXT_TIMEOUT)));
        settings.insert(QString("useProxies"), Settings::instance().value(QStringLiteral(TEXT_USE_PROXIES)));
        switch (currentTool.id())
        {
            case Tools::CHECK_URL_STATUS:
                worker = new CheckUrlStatusWorker(workerId, &m_inputDataQueue, &m_mutex, settings);
                break;
            case Tools::CHECK_ALEXA_RANK:
                worker = new CheckAlexaRankWorker(workerId, &m_inputDataQueue, &m_mutex, settings);
                break;
            case Tools::SCRAPE_PROXIES:
                worker = new ScrapeProxiesWorker(workerId, &m_inputDataQueue, &m_mutex, settings);
                break;
            case Tools::TEST:
                worker = new TestWorker(workerId, &m_inputDataQueue, &m_mutex, settings);
                break;
            default:
                worker = new DummyWorker(workerId, &m_inputDataQueue, &m_mutex, settings);
        }
        m_threads.append(thread);
        m_workers.append(worker);
        m_workers[i]->moveToThread(m_threads[i]);

        connect(thread, &Thread::started, worker, &Worker::run);
        connect(thread, &Thread::finished, thread, &Thread::deleteLater);
        connect(worker, &Worker::result, this, &WorkspaceWidget::onResult);
        connect(worker, &Worker::status, this, &WorkspaceWidget::onStatus);
        connect(worker, &Worker::finished, thread, &Thread::quit);
        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
        connect(worker, &Worker::finished, []{
            qDebug() << "Worker finished";
        });
        connect(worker, &Worker::requestStop, worker, &Worker::stop);
        connect(worker, &Worker::log, m_logWidget, &LogWidget::onLog);
    }

    Q_EMIT jobStarted();
    m_tablesWidget->switchToResultsTab();
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
            Q_EMIT worker->requestStop();
        }
    }
    Q_EMIT jobStopped();
}

void WorkspaceWidget::onResult(const QVariantMap& resultData)
{
    ++m_itemsDone;
    if (m_itemsDone == 1)
        m_tablesWidget->focusedTable()->resizeColumns();
//     Tools currentToolId = static_cast<Tools>(resultData["toolId"].toInt());
    auto currentToolName = resultData["toolName"].toString();
    Tool currentTool = m_toolsWidget->getTool(currentToolName);
//     int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    QStringList row;
    for (const auto& column : currentTool.columns())
    {
//        if (column == QString("Status"))
//        {
//            ResultStatus status = static_cast<ResultStatus>(resultData["Status"].toInt());
//            switch (status)
//            {
//                case ResultStatus::OK:
//                    row << "Done";
//                    break;
//                case ResultStatus::PROCESSING:
//                    row << "...";
//                    break;
//                case ResultStatus::FAILED:
//                    row << "Failed";
//                    break;
//            }
//        }
//        else
//        {
        row << resultData[column].toString();
//        }
    }
    m_tablesWidget->resultsTable()->appendRow(row);
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    m_progressBar->setValue(progresPercentage);
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
