// #include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QDebug>
// #include <QDesktopServices>
// // #include <QDesktopWidget>
#include <QDir>
#include <QHBoxLayout>
// #include <QFile>
#include <QFileDialog>
// #include <QIcon>
// #include <QIODevice>
// #include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
// #include <QModelIndex>
// #include <QPair>
// #include <QPoint>
// #include <QProgressBar>
#include <QPushButton>
// #include <QRect>
#include <QSettings>
// #include <QSpinBox>
// #include <QStandardItem>
// #include <QStandardItemModel>
#include <QStackedWidget>
#include <QStatusBar>
// #include <QStringList>
// #include <QTextStream>
// #include <QTextEdit>
#include <QTimer>
// #include <QTreeWidget>
// #include <QMainWindow>
// #include <QTableView>
#include <QToolBar>
// #include <QVBoxLayout>
// #include <QWidget>
// #include <QGridLayout>
// #include <QThread>
// #include <QRegExp>
// #include <QTabWidget>

#include "mainwindow.h"
#include "helpwidget.h"
#include "proxieswidget.h"
#include "settingswidget.h"
#include "sidebar.h"
#include "toolswidget.h"
#include "workspacewidget.h"
#include "../config.h"
#include "../constants.h"
#include "../icons.h"
#include "../version.h"
// #include "../common/table.h"
#include "../common/applicationstatemachine.h"
#include "../common/basemainwindow.h"
#include "../common/recentfiles.h"
#include "../common/table.h"
#include "../common/thread.h"
// #include "../core/sidebar.h"
// #include "proxieswidget.h"
// #include "toolswidget.h"
#include "../workers/worker.h"
// #include "../workers/checkurlstatusworker.h"
// #include "../workers/checkalexarank.h"
// #include "../workers/scrapeproxies.h"
#include "../workers/dummyworker.h"
// #include "settingswidget.h"
// #include "workspacewidget.h"
// #include "../core/tools.h"
// #include "../core/tool.h"
// #include "../utils/useragents.h"
#include "../utils/file.h"


MainWindow::MainWindow ( QWidget* parent ) : BaseMainWindow(parent)
{
    setWindowTitle(QStringLiteral("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));

    QDir applicationDir(QApplication::applicationDirPath());
    m_settingsFilePath = applicationDir.absoluteFilePath("settings.ini");
    m_lastDirectory = applicationDir.absolutePath();

//     m_columnRatios << 0.5 << 0.2 << 0.2;
    m_pulseTimer = new QTimer(this);
    m_recentFiles = new RecentFiles(5, this);

    m_threads = QList<Thread*>();
    m_workers = QList<Worker*>();
    m_inputDataQueue = QQueue<QMap<QString, QVariant>>();

    m_applicationStateMachine = new ApplicationStateMachine(this);
    m_applicationStateMachine->start();

    createActions();
    createMenuBar();
    createToolBar();
    createWidgets();
    createStatusBar();
    createConnections();

    if (QFile::exists(m_settingsFilePath))
    {
        loadSettings();
    }
    else
    {
        resize(800, 600);
        centerWindow();
    }

    // Init recent files
    for (QAction *action : m_recentFiles->actions())
    {
        m_recentUrlFilesMenu->addAction(action);
    }
/*
    m_userAgents.addUserAgent(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36"));;
    m_userAgents.addUserAgent(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:85.0) Gecko/20100101 Firefox/85.0"));;
    m_userAgents.addUserAgent(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.104 Safari/537.36"));;
    m_userAgents.addUserAgent(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36"));;
    m_userAgents.addUserAgent(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36"));;*/

    m_pulseTimer->start(1 * MILLIS_IN_SECOND);
    QTimer::singleShot(5 * MILLIS_IN_SECOND, [this]{
        emit m_applicationStateMachine->applicationReady();
    });

    // TODO: Remove this
    Table* inputTable = m_workspaceWidget->inputTable();
    for (auto& line : File::readTextLines("/mnt/ramdisk/urls.txt"))
    {
        line = line.trimmed();
        // TODO: validate URL
        if (line.length() > 0)
            inputTable->appendRow(QStringList() << line << "");
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    m_importUrlsAction = new QAction(QIcon(ICON_TABLE_IMPORT), "Import URLs", this);
    m_recentUrlFilesMenu = new QMenu("Open Recent URL File", this);
    m_clearRecentUrlFilesAction = new QAction(QIcon(ICON_BROOM), "Clear List", this);
    m_exportResultsAction = new QAction(QIcon(ICON_TABLE_EXPORT), "Export Results", this);
    m_quitAction = new QAction(QIcon(ICON_CROSS_CIRCLE), "Quit", this);
    m_clearTableAction = new QAction(QIcon(ICON_BROOM), "Clear Results Table", this);
    m_removeDuplicatesAction = new QAction(QIcon(ICON_TABLE_DELETE_ROW), "Remove Duplicates", this);
    m_selectAllAction = new QAction(QIcon(ICON_TABLE_SELECT_ALL), "Select All Rows", this);
    m_invertSelectionAction = new QAction(QIcon(ICON_TABLE), "Invert Selection", this);
    m_removeSelectedAction= new QAction(QIcon(ICON_TABLE_DELETE_ROW), "Remove Selected Rows", this);
    m_centerWindowAction = new QAction(QIcon(ICON_RESIZE), "Center Window", this);
    m_aboutAction = new QAction(QIcon(ICON_INFORMATION), "About", this);

    // Sidebar
    m_workspaceAction = new QAction(QIcon(ICON_DESKTOP), "Workspace", this);
    m_settingsAction = new QAction(QIcon(ICON_GEAR), "Settings", this);
    m_proxiesAction = new QAction(QIcon(ICON_MASK), "Proxies", this);
    m_helpAction = new QAction(QIcon(ICON_QUESTION), "Help", this);
}

void MainWindow::createMenuBar()
{
    // Groups
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_selectionMenu = menuBar()->addMenu(tr("Selection"));
    m_windowMenu = menuBar()->addMenu(tr("Window"));
    m_helpMenu = menuBar()->addMenu(tr("Help"));
    // File menu
    m_fileMenu->addAction(m_importUrlsAction);
    m_fileMenu->addMenu(m_recentUrlFilesMenu);
    m_recentUrlFilesMenu->addAction(m_clearRecentUrlFilesAction);
    m_fileMenu->addAction(m_exportResultsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
    // Edit menu
    m_editMenu->addAction(m_clearTableAction);
    m_editMenu->addAction(m_removeDuplicatesAction);
    // Seletion menu
    m_selectionMenu->addAction(m_removeSelectedAction);
    m_selectionMenu->addAction(m_invertSelectionAction);
    m_selectionMenu->addAction(m_removeSelectedAction);
    // Window menu
    m_windowMenu->addAction(m_centerWindowAction);
    // Help menu
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBar()
{
    // Toolbar
    m_toolBar = addToolBar("mainToolBar");
    m_toolBar->setObjectName("mainToolBar");
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
    // Buttons
    m_toolBar->addAction(m_importUrlsAction);
    m_toolBar->addAction(m_exportResultsAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_clearTableAction);
    m_toolBar->addAction(m_removeDuplicatesAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_selectAllAction);
    m_toolBar->addAction(m_invertSelectionAction);
    m_toolBar->addAction(m_removeSelectedAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_quitAction);
}

void MainWindow::createWidgets()
{
    m_centralWidget = new QWidget;
    // Sidebar
    m_sideBar = new SideBar;
    m_sideBar->addAction(m_workspaceAction, true);
    m_sideBar->addAction(m_settingsAction);
    m_sideBar->addAction(m_proxiesAction);
    m_sideBar->addAction(m_helpAction);
    // Main pages
    m_mainStackedWidget = new QStackedWidget;
    m_workspaceWidget = new WorkspaceWidget;
    m_settingsWidget = new SettingsWidget;
    m_proxiesWidget = new ProxiesWidget;
    m_helpWidget = new HelpWidget;

    m_mainStackedWidget->addWidget(m_workspaceWidget);
    m_mainStackedWidget->addWidget(m_settingsWidget);
    m_mainStackedWidget->addWidget(m_proxiesWidget);
    m_mainStackedWidget->addWidget(m_helpWidget);

    m_centralLayout = new QHBoxLayout(m_centralWidget);
    m_centralLayout->addWidget(m_sideBar);
    m_centralLayout->addWidget(m_mainStackedWidget);

    setCentralWidget(m_centralWidget);
}

void MainWindow::createStatusBar()
{
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);

    m_toolsPushButton = new QPushButton(QIcon(ICON_HAMMER), "");
    m_statusBarLabel = new QLabel;
    m_activeThreadsLabel = new QLabel(" Active threads: /");

//     connect(m_toolsPushButton, &QPushButton::clicked, m_workspaceWidget, &WorkspaceWidget::toggleTools);

    m_statusBar->addPermanentWidget(m_toolsPushButton);
    m_statusBar->addPermanentWidget(m_statusBarLabel, 1);
    m_statusBar->addPermanentWidget(m_activeThreadsLabel);
}

void MainWindow::createConnections()
{
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::close);
    connect(m_aboutAction, &QAction::triggered, [&] {QMessageBox::about(this,
        "About " APP_TITLE,

        "<p>Fugue icons are provided by <a href='http://p.yusukekamiyamane.com/'>Yusuke"
        "Kamiyamane</a>"
    );});
    // Sidebar
    connect(m_workspaceAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(0);});
    connect(m_settingsAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(1);});
    connect(m_proxiesAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(2);});
    connect(m_helpAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(3);});
    // File
    connect(m_importUrlsAction, &QAction::triggered, this, &MainWindow::importUrls);
    connect(m_exportResultsAction, &QAction::triggered, this, &MainWindow::exportResults);
    
    
    
    //     connect(m_workspaceWidget, &WorkspaceWidget::test, [this]{
//         qDebug() << "test";
//         qDebug() << m_workspaceWidget->toolsWidget()->currentTool().name();
//     });
// //     connect(m_testPushButton, &QPushButton::clicked, [this]{
// //         
// //         qDebug() << m_userAgents.get();
// //     });

    connect(m_pulseTimer, &QTimer::timeout, this, &MainWindow::onPulse);


//     connect(m_centerWindowAction, &QAction::triggered, this, &MainWindow::centerWindow);


// 
// //     connect(m_clearTableAction, &QAction::triggered, m_resultsTable, &Table::removeAllRows);
// //     connect(m_selectAllAction, &QAction::triggered, m_resultsTable, &Table::selectAll);
// //     connect(m_invertSelectionAction, &QAction::triggered, m_resultsTable, &Table::invertSelection);
// //     connect(m_removeDuplicatesAction, &QAction::triggered, m_resultsTable, &Table::removeDuplicates);
// //     connect(m_removeSelectedAction, &QAction::triggered, m_resultsTable, &Table::removeSelected);
// 
//     connect(m_workspaceWidget, &WorkspaceWidget::startJob, this, &MainWindow::startJob);
//     connect(m_workspaceWidget, &WorkspaceWidget::stopJob, this, &MainWindow::stopJob);
// //     connect(m_startPushButton, &QPushButton::clicked, this, &MainWindow::startJob);
// //     connect(m_stopPushButton, &QPushButton::clicked, this, &MainWindow::stopJob);
// //     connect(m_resultsTable, &Table::doubleClicked, [this] (const QModelIndex &modelIndex) {
// //         QDesktopServices::openUrl(QUrl(m_resultsTable->cell(modelIndex.row(), 0).toString()));
// //     });
//     connect(m_recentFiles, &RecentFiles::filePathSelected, this, &MainWindow::importRecentFileUrls);
// 
//     connect(m_applicationStateMachine, &ApplicationStateMachine::applicationStarted, this, &MainWindow::onApplicationStart);
//     connect(m_applicationStateMachine, &ApplicationStateMachine::applicationIdling, this, &MainWindow::onApplicationReady);
//     connect(m_applicationStateMachine, &ApplicationStateMachine::applicationExiting, this, &MainWindow::onApplicationExit);
//     connect(m_applicationStateMachine, &ApplicationStateMachine::jobStarted, this, &MainWindow::onJobStart);
//     connect(m_applicationStateMachine, &ApplicationStateMachine::jobStopping, this, &MainWindow::onJobStop);
//     connect(m_applicationStateMachine, &ApplicationStateMachine::jobFinished, this, &MainWindow::onJobDone);
}

void MainWindow::loadSettings()
{
    if (QFile::exists(m_settingsFilePath))
    {
        QSettings settings(m_settingsFilePath, QSettings::IniFormat);
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("windowState").toByteArray());
        m_lastDirectory = settings.value("lastDirectory", m_lastDirectory).toString();
        m_proxiesWidget->setPlainText(settings.value("proxies").toString());
    }
}

void MainWindow::saveSettings()
{
    QSettings settings(m_settingsFilePath, QSettings::IniFormat);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("lastDirectory", m_lastDirectory);
    settings.setValue("proxies", m_proxiesWidget->toPlainText());
}

// Events
void MainWindow::closeEvent(QCloseEvent* event)
{
//    emit m_applicationState->applicationExiting();
    saveSettings();
    QMainWindow::closeEvent(event);
}

// void MainWindow::resizeEvent(QResizeEvent* event)
// {
// //     m_resultsTable->resizeColumns();
//     QMainWindow::resizeEvent(event);
// }
// 

void MainWindow::onApplicationStart()
{
//     auto currentTool = m_workspaceWidget->toolsWidget()->currentTool();
//     m_toolsPushButton->setIcon(currentTool.icon());
//     m_toolsPushButton->setText(currentTool.name());
//     m_startPushButton->setEnabled(false);
//     m_stopPushButton->setEnabled(false);
}

void MainWindow::onApplicationReady()
{
    auto currentTool = m_workspaceWidget->toolsWidget()->currentTool();
    m_toolsPushButton->setIcon(currentTool.icon());
    m_toolsPushButton->setText(currentTool.name());
//     m_startPushButton->setEnabled(true);
//     m_stopPushButton->setEnabled(false);
}

void MainWindow::onApplicationExit()
{
//     m_startPushButton->setEnabled(false);
//     m_stopPushButton->setEnabled(false);
}

void MainWindow::onJobStart()
{
//     m_startPushButton->setEnabled(false);
//     m_stopPushButton->setEnabled(true);
}

void MainWindow::onJobStop()
{
//     m_startPushButton->setEnabled(false);
//     m_stopPushButton->setEnabled(false);
}

void MainWindow::onJobDone()
{
//     m_startPushButton->setEnabled(false);
//     m_stopPushButton->setEnabled(false);
}

// Slots
void MainWindow::onPulse()
{
    m_activeThreadsLabel->setText(QString(" Active threads: %1").arg(Thread::count()));
}

void MainWindow::importUrls()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Import URLs", m_lastDirectory, "Text files (*.txt);;All files (*.*)");
    if (!QFile::exists(filePath))
        return;
    for (auto& line : File::readTextLines(filePath))
    {
        line = line.trimmed();
        // TODO: validate URL
//         if (line.length() > 0)
//             m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
    }
    m_lastDirectory = QDir(filePath).absolutePath();
    m_recentFiles->addFile(filePath);
}

void MainWindow::exportResults()
{
    QString filePath(QDir(m_lastDirectory).absoluteFilePath("results.txt"));
    filePath = QFileDialog::getSaveFileName(this, "Export Results", filePath, "Text files (*.txt)");
    QStringList urls;
//     int columnCount = 4;
    Table* resultsTable = m_workspaceWidget->resultsTable();
    int rowCount = resultsTable->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        urls << resultsTable->cell(i, 0).toString();
    }
    if (urls.length() == 0)
    {
        QMessageBox::information(this, "Export Results", "Nothing to export!");
        return;
    }
    File::writeTextFile(filePath, urls);
    m_lastDirectory = QDir(filePath).absolutePath();
}

void MainWindow::importRecentFileUrls(const QString& filePath)
{
    if (!QFile::exists(filePath))
        return;
    for (auto& line : File::readTextLines(filePath))
    {
        line = line.trimmed();
        // TODO: validate URL
//         if (line.length() > 0)
//             m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
    }
    m_lastDirectory = QDir(filePath).absolutePath();
}

void MainWindow::startJob()
{
    m_itemsDone = 0;
//     m_totalItems = m_resultsTable->rowCount();
    Table* resultsTable = m_workspaceWidget->resultsTable();
    m_totalItems = resultsTable->rowCount();
    m_threads.clear();
    m_workers.clear();
    for (int i = 0; i < resultsTable->rowCount(); ++i)
    {
        auto url = resultsTable->cell(i, 0).toString();
        m_inputDataQueue.enqueue({
            {QString("rowId"), QVariant(i)},
            {QString("url"), QVariant(url)}
        });
    }
    int parallelTasks = m_settingsWidget->threadCount();
    for (int i = 0; i < parallelTasks;++i)
    {
//         auto thread = new QThread;
        auto thread = new Thread;
        Worker *worker;
        // TODO: Improve tool switching logic
//         if (m_toolsPushButton->text() == " Check URL Status")
//             worker = new CheckUrlStatusWorker(m_inputDataQueue);
//         else if (m_toolsPushButton->text() == " Check Alexa Rank")
//             worker = new CheckAlexaRankWorker(m_inputDataQueue);
//         else if (m_toolsPushButton->text() == " Scrape Proxies")
//             worker = new ScrapeProxiesWorker(m_inputDataQueue);
//         else
//             worker = new DummyWorker(m_inputDataQueue);
        worker = new DummyWorker(m_inputDataQueue);
        m_threads.append(thread);
        m_workers.append(worker);
        m_workers[i]->moveToThread(m_threads[i]);
        // Connections
        connect(thread, &Thread::started, worker, &Worker::run);
        connect(thread, &Thread::finished, thread, &Thread::deleteLater);
        connect(worker, &Worker::result, this, &MainWindow::onResult);
        connect(worker, &Worker::finished, thread, &Thread::quit);
        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
        connect(worker, &Worker::finished, []{
            qDebug() << "Worker finished";
        });
        connect(worker, &Worker::requestStop, worker, &Worker::stop);
    }

    emit m_applicationStateMachine->jobStart();
    for (int i = 0; i < parallelTasks; ++i)
    {
        m_threads[i]->start();
    }
}

void MainWindow::stopJob()
{
    for (Worker *worker: m_workers)
    {
        if (worker)
        {
            emit worker->requestStop();
        }
    }
    emit m_applicationStateMachine->jobStop();
}

void MainWindow::onResult(const QMap<QString, QVariant>& resultData)
{
    ++m_itemsDone;
    m_workspaceWidget->updateResultsRow(resultData);
    int progresPercentage = static_cast<int>(static_cast<double>(m_itemsDone) / m_totalItems * 100);
    m_workspaceWidget->setCurrentProgress(progresPercentage);
}
