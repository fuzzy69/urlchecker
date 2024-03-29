﻿#include "workspacewidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTableView>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>

//#include "../core/applicationbridge.h"
#include "../core/applicationstatemachine.h"
#include "../core/settings.h"
#include "../core/table.h"
#include "../core/workermanager.h"
#include "../icons.h"
#include "../texts.h"
#include "../tools/workerfactory.h"
#include "../widgets/filesystemwidget.h"
#include "../widgets/logwidget.h"
#include "../widgets/statusbarwidget.h"
#include "../widgets/tableswidget.h"
#include "toolswidget.h"

#include "../core/toolsmanager.h"

#include "../core/worker.h"

WorkspaceWidget::WorkspaceWidget(QWidget* parent)
    : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(2);

    m_topLayout = new QHBoxLayout;
    m_bottomLayout = new QHBoxLayout;
    m_horizontalSplitter = new QSplitter(Qt::Horizontal);
    m_verticalSplitter = new QSplitter(Qt::Vertical);

    m_toolsWidget = new ToolsWidget;
    m_sideTabWidget = new QTabWidget;
    m_sideTabWidget->setContentsMargins(0, 0, 0, 0);
    m_tablesWidget = new TablesWidget;
    m_fileSystemWidget = new FilesystemWidget;
    m_logWidget = new LogWidget;
    m_startPushButton = new QPushButton(QIcon(ICON_CONTROL), tr("Start"));
    m_stopPushButton = new QPushButton(QIcon(ICON_CONTROL_STOP), tr("Stop"));
    //    m_testPushButton = new QPushButton(tr("Test"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_sideTabWidget->addTab(m_toolsWidget, QIcon(ICON_HAMMER), tr("Tools"));
    m_sideTabWidget->addTab(m_fileSystemWidget, QIcon(ICON_DRIVE), tr("Filesystem"));
    m_horizontalSplitter->addWidget(m_sideTabWidget);
    m_horizontalSplitter->addWidget(m_tablesWidget);
    m_horizontalSplitter->setSizes(QList<int>({ 300, 1000 }));
    m_verticalSplitter->addWidget(m_horizontalSplitter);
    m_verticalSplitter->addWidget(m_logWidget);
    m_verticalSplitter->setSizes(QList<int>({ 1000, 200 }));
    m_topLayout->addWidget(m_verticalSplitter);
    m_bottomLayout->addWidget(m_progressBar);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    //    m_bottomLayout->addWidget(m_testPushButton);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_bottomLayout);

    m_workerManager = new WorkerManager(this);

    //    ApplicationBridge::instance().setToolsWidget(m_toolsWidget);
    //    ApplicationBridge::instance().setTablesWidget(m_tablesWidget);
    //    ApplicationBridge::instance().setLogWidget(m_logWidget);
    //    ApplicationBridge::instance().setProgressBar(m_progressBar);

    connect(m_startPushButton, &QPushButton::clicked, this, &WorkspaceWidget::startJob);
    connect(m_stopPushButton, &QPushButton::clicked, this, &WorkspaceWidget::stopJob);
    connect(m_workerManager, &WorkerManager::result, this, &WorkspaceWidget::onResult);
    connect(m_workerManager, &WorkerManager::status, this, &WorkspaceWidget::onStatus);

    connect(m_workerManager, &WorkerManager::jobStarted, ApplicationStateMachine::self(), &ApplicationStateMachine::jobStart);
    connect(m_workerManager, &WorkerManager::jobStopped, ApplicationStateMachine::self(), &ApplicationStateMachine::jobStop);
    connect(m_workerManager, &WorkerManager::progress, [this](const int itemsSuccessfullyDone, const int itemsDone, const int totalItems, const double progressPercentage) {
        Q_UNUSED(itemsSuccessfullyDone)
        Q_UNUSED(itemsDone)
        Q_UNUSED(totalItems)
        m_progressBar->setValue(static_cast<int>(progressPercentage));
        //        double successRatio = static_cast<double>(itemsSuccessfullyDone) / itemsDone * 100.;
        //        ApplicationBridge::instance().statusBarWidget()->setJobStatsStatus(QString(" Completed %1 / %2 of %3 items. Success ratio %4% ").arg(itemsSuccessfullyDone).arg(itemsDone).arg(totalItems).arg(successRatio, 0, 'f', 1));
    });
    connect(m_workerManager, &WorkerManager::log, m_logWidget, &LogWidget::onLog);

    // Application states
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::applicationStarted, this, &WorkspaceWidget::onApplicationStart);
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::applicationIdling, [this]() {
        onApplicationReady();
    });
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::applicationExiting, this, &WorkspaceWidget::onApplicationExit);
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::jobStarted, this, &WorkspaceWidget::onJobStart);
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::jobStopping, this, &WorkspaceWidget::onJobStop);
    connect(ApplicationStateMachine::self(), &ApplicationStateMachine::jobFinished, this, &WorkspaceWidget::onJobDone);

    qRegisterMetaType<ResultStatus>("ResultStatus");
    qRegisterMetaType<Tools>("Tools");
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

ToolsWidget* WorkspaceWidget::toolsWidget()
{
    return m_toolsWidget;
}

FilesystemWidget* WorkspaceWidget::filesystemWidget()
{
    return m_fileSystemWidget;
}

TablesWidget* WorkspaceWidget::tablesWidget()
{
    return m_tablesWidget;
}

LogWidget* WorkspaceWidget::logWidget()
{
    return m_logWidget;
}

WorkerManager* WorkspaceWidget::workerManager()
{
    return m_workerManager;
}

void WorkspaceWidget::setCurrentProgress(int value)
{
    m_progressBar->setValue(value);
}

QTabWidget* WorkspaceWidget::sideTabWidget()
{
    return m_sideTabWidget;
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
    if (m_tablesWidget->inputTable()->rowCount() == 0) {
        QMessageBox::warning(this, QStringLiteral("Start Job"), QStringLiteral("Nothing to do, source URLs table is empty!"));
        return;
    }
    auto& currentTool = ToolsManager::instance().currentTool();
    // Clear input's table status column
    m_tablesWidget->inputTable()->setColumn(1, QVariant(QStringLiteral("")));
    m_tablesWidget->resultsTable()->resetColumns(currentTool.columns());
    m_tablesWidget->resultsTable()->setColumnRatios(currentTool.columnRatios());
    Table* inputTable = m_tablesWidget->inputTable();
    m_workerManager->inputData().clear();
    for (int i = 0; i < inputTable->rowCount(); ++i) {
        QString url(inputTable->cell(i, 0).toString());
        m_workerManager->inputData().enqueue({ { QString("rowId"), QVariant(i) },
            { QString("url"), QVariant(url) } });
    }
    QList<Worker*> workers;
    QVariantMap currentSettings = Settings::instance().get();
    const int threadCount = currentSettings.value(QStringLiteral(TEXT_THREADS)).toInt();
    for (int i = 0; i < threadCount; ++i) {
        workers.append(workerFactory(currentTool.id(), i + 1, &(m_workerManager->inputData()), &(m_workerManager->lock()), currentSettings));
    }
    m_workerManager->startJob(workers);
    m_tablesWidget->switchToResultsTab();
    m_tablesWidget->resultsTable()->setContextMenu(currentTool.contextMenu());
}

void WorkspaceWidget::stopJob()
{
    m_workerManager->stopJob();
}

void WorkspaceWidget::onResult(Tools toolId, const QVariantMap& resultData)
{
    auto* currentTool = ToolsManager::instance().getTool(toolId);
    QStringList row;
    for (const auto& column : currentTool->columns()) {
        row << resultData[column].toString();
    }
    m_tablesWidget->resultsTable()->appendRow(row);
    if (m_tablesWidget->focusedTable()->rowCount() == 1)
        m_tablesWidget->focusedTable()->resizeColumns();
}

void WorkspaceWidget::onStatus(const int rowId, const ResultStatus& resultStatus)
{
    QVariant status(ResultStatusText.value(resultStatus));
    m_tablesWidget->inputTable()->setCell(rowId, 1, status);
    //    switch (resultStatus) {
    //    case ResultStatus::OK:
    //        m_tablesWidget->inputTable()->setRowColor(rowId, Qt::white, Qt::green);
    //        break;
    //    case ResultStatus::PROCESSING:
    //        m_tablesWidget->inputTable()->setRowColor(rowId, Qt::white, Qt::blue);
    //        break;
    //    case ResultStatus::FAILED:
    //        m_tablesWidget->inputTable()->setRowColor(rowId, Qt::white, Qt::red);
    //        break;
    //        //    default:
    //        //        m_tablesWidget->inputTable()->setRowColor(rowId, Qt::black, Qt::white);
    //        //        break;
    //    }
}
