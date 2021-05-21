#include <string>

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>

#include "my/browserutils.h"
#include "my/file.h"
#include "my/httpproxy.h"
#include "my/proxymanager.h"
#include "my/proxyutils.h"
#include "my/urlutils.h"
#include "my/useragents.h"

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
#include "../common/applicationstatemachine.h"
#include "../common/basemainwindow.h"
#include "../common/recentfiles.h"
#include "../common/settings.h"
#include "../common/table.h"
#include "../common/thread.h"
#include "../workers/worker.h"
#include "../workers/dummyworker.h"
#include "../config.h"

using my::filesystem::File;
using my::network::HttpProxy;
using my::network::ProxyManager;
using my::browser::UserAgentsManager;
using my::data::USER_AGENTS_TEXT;

MainWindow::MainWindow ( QWidget* parent ) : BaseMainWindow(parent)
{
    setWindowTitle(QStringLiteral("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));

    QDir applicationDir(QApplication::applicationDirPath());
    m_lastDirectory = applicationDir.absolutePath();

    // Files
    m_settingsFilePath = applicationDir.filePath("settings.json");
    m_proxiesFilePath = applicationDir.filePath("proxies.txt");
    m_userAgentsFilePath = applicationDir.filePath("user_agents.txt");

    Settings::instance().setFilePath(m_settingsFilePath);
    // Init settings
    Settings::instance().setValue("parallelTasks", QVariant(PARALLEL_TASKS));
    Settings::instance().setValue("timeout", QVariant(REQUEST_TIMEOUT));
    Settings::instance().setValue("useProxies", QVariant(USE_PROXIES));
    Settings::instance().setValue("lastDirectory", QVariant(m_lastDirectory));

    m_pulseTimer = new QTimer(this);
    m_recentFiles = new RecentFiles(5, this);

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

    m_pulseTimer->start(1 * MILLIS_IN_SECOND);
    QTimer::singleShot(3 * MILLIS_IN_SECOND, [this]{
        emit m_applicationStateMachine->applicationReady();
    });

    Table* inputTable = m_workspaceWidget->inputTable();
    // User agents
    if (!QFile::exists(m_userAgentsFilePath))
    {
        File::writeTextFile(m_userAgentsFilePath, QString(USER_AGENTS_TEXT));
    }
    for (auto& line : File::readTextLines(m_userAgentsFilePath))
    {
        UserAgentsManager<QString>::instance().add_user_agent(line.trimmed());
    }
    // Proxies
    for (auto& proxy : my::network::loadProxiesFromFile(m_proxiesFilePath))
    {
        ProxyManager::instance().add_proxy(proxy);
        m_proxiesWidget->append(QString::fromStdString(proxy));
    }
    // TODO: Remove this
    for (const auto& url : my::url::loadUrlsFromFile("/mnt/ramdisk/proxy_sources.txt"))
    {
        inputTable->appendRow(QStringList() << url.toString() << "");
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
    m_quitAction = new QAction(QIcon(":/assets/icons/control-power.png"), "Quit", this);
    m_removeAllAction = new QAction(QIcon(ICON_BROOM), "Remove All Rows", this);
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
    m_recentUrlFilesMenu->addSeparator();
    m_fileMenu->addAction(m_exportResultsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
    // Edit menu
    m_editMenu->addAction( m_removeAllAction );
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
    m_toolBar->addAction( m_removeAllAction );
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

    m_statusBar->addPermanentWidget(m_toolsPushButton);
    m_statusBar->addPermanentWidget(m_statusBarLabel, 1);
    m_statusBar->addPermanentWidget(m_activeThreadsLabel);
}

void MainWindow::createConnections()
{
    // File menu
    connect(m_importUrlsAction, &QAction::triggered, this, &MainWindow::importUrls);
    connect(m_exportResultsAction, &QAction::triggered, this, &MainWindow::exportResults);
    connect(m_recentFiles, &RecentFiles::filePathSelected, this, &MainWindow::importRecentFileUrls);
    connect(m_clearRecentUrlFilesAction, &QAction::triggered, [this]{
        m_recentFiles->clear();
    });
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::close);
    // Window
    connect(m_centerWindowAction, &QAction::triggered, this, &MainWindow::centerWindow);
    // Help menu
    connect(m_aboutAction, &QAction::triggered, [&] {QMessageBox::about(this,
        QString("About %1").arg(APP_TITLE),
        QString(
            "<h3>%1 %2</h3><br/>"
            "%3<br/>"
            "Fugue icons are provided by <a href='http://p.yusukekamiyamane.com/'>Yusuke"
        "Kamiyamane</a>"
        ).arg(APP_TITLE, APP_VERSION, APP_DESCRIPTION)
    );});

    // Sidebar
    connect(m_workspaceAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(0);});
    connect(m_settingsAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(1);});
    connect(m_proxiesAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(2);});
    connect(m_helpAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(3);});

    // Workspace
    connect(m_removeAllAction, &QAction::triggered, m_workspaceWidget, &WorkspaceWidget::removeAllRows);
    connect(m_selectAllAction, &QAction::triggered, m_workspaceWidget, &WorkspaceWidget::selectAllRows);
    connect(m_invertSelectionAction, &QAction::triggered, m_workspaceWidget, &WorkspaceWidget::invertSelectedRows);
    connect(m_removeDuplicatesAction, &QAction::triggered, m_workspaceWidget, &WorkspaceWidget::removeDuplicatedRows);
    connect(m_removeSelectedAction, &QAction::triggered, m_workspaceWidget, &WorkspaceWidget::removeSelectedRows);
    connect(m_workspaceWidget, &WorkspaceWidget::jobStarted, m_applicationStateMachine, &ApplicationStateMachine::jobStart);
    connect(m_workspaceWidget, &WorkspaceWidget::jobStopped, m_applicationStateMachine, &ApplicationStateMachine::jobStop);

    // Statusbar
    connect(m_toolsPushButton, &QPushButton::clicked, m_workspaceWidget, &WorkspaceWidget::toggleTools);
    connect(m_workspaceWidget->toolsWidget(), &ToolsWidget::toolSelected, [this](const Tool &currentTool){
        m_toolsPushButton->setIcon(currentTool.icon());
        m_toolsPushButton->setText(currentTool.name());
    });

    // Application states
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationStarted, this, &MainWindow::onApplicationStart);
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationIdling, this, &MainWindow::onApplicationReady);
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationExiting, this, &MainWindow::onApplicationExit);
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobStarted, this, &MainWindow::onJobStart);
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobStopping, this, &MainWindow::onJobStop);
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobFinished, this, &MainWindow::onJobDone);

    // Misc
    connect(m_pulseTimer, &QTimer::timeout, this, &MainWindow::onPulse);

    // Test
    connect(m_workspaceWidget, &WorkspaceWidget::test, [this]{
        qDebug() << Settings::instance().value("parallelTasks");
    });
}

void MainWindow::loadSettings()
{
    if (QFile::exists(m_settingsFilePath))
    {
        Settings::instance().load();
        restoreGeometry(QByteArray::fromHex(Settings::instance().value("geometry").toByteArray()));
        restoreState(QByteArray::fromHex(Settings::instance().value("windowState").toByteArray()));
        m_lastDirectory = Settings::instance().value("lastDirectory").toString();
        // m_proxiesWidget->setPlainText(Settings::instance().value("proxies").toString());
    }
}

void MainWindow::saveSettings()
{
    Settings::instance().setValue("geometry", saveGeometry().toHex());
    Settings::instance().setValue("windowState", saveState().toHex());
    Settings::instance().setValue("lastDirectory", m_lastDirectory);
    // Settings::instance().setValue("proxies", m_proxiesWidget->toPlainText());
    Settings::instance().save();
}

// Events
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_applicationStateMachine->currentState() == ApplicationState::JOB_RUNNING)
    {
        auto reply = QMessageBox::question(this, "Close Confirmation", "Some jobs are stil running, close anyway?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }
    // TODO: Stop remaining running workers/threads
    emit m_applicationStateMachine->applicationExiting();
    saveSettings();
    // Save proxies to file
    File::writeTextFile(m_proxiesFilePath, m_proxiesWidget->toPlainText());
    // saveProxiesToFile(m_proxiesFilePath);
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
    m_workspaceWidget->onApplicationReady();
}

void MainWindow::onApplicationReady()
{
    auto currentTool = m_workspaceWidget->toolsWidget()->currentTool();
    m_toolsPushButton->setIcon(currentTool.icon());
    m_toolsPushButton->setText(currentTool.name());
    m_workspaceWidget->onApplicationReady();
    setStatusMessage("Ready");
}

void MainWindow::onApplicationExit()
{
    m_workspaceWidget->onApplicationExit();
}

void MainWindow::onJobStart()
{
    m_workspaceWidget->onJobStart();
}

void MainWindow::onJobStop()
{
    m_workspaceWidget->onJobStop();
}

void MainWindow::onJobDone()
{
    m_workspaceWidget->onJobDone();
}

// Slots
void MainWindow::onPulse()
{
    m_activeThreadsLabel->setText(QString(" Active threads: %1").arg(Thread::count()));
    if (m_applicationStateMachine->currentState() == ApplicationState::JOB_RUNNING && Thread::count() == 0)
        emit m_applicationStateMachine->jobDone();
}

void MainWindow::importUrlFile(const QString &filePath)
{
    if (!QFile::exists(filePath))
        return;
    if (m_workspaceWidget->inputTable()->rowCount() != 0)
    {
        QMessageBox messageBox(QMessageBox::Question, "Import URLs", "Input table not empty! Append to existing rows or replace current rows?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        messageBox.setButtonText(QMessageBox::Yes, "Append");
        messageBox.setButtonText(QMessageBox::No, "Replace");
        int pushedButton = messageBox.exec();
        if (pushedButton == QMessageBox::Cancel)
            return;
        if (pushedButton == QMessageBox::No)
            m_workspaceWidget->inputTable()->removeAllRows();
    }
    Table* inputTable = m_workspaceWidget->inputTable();
    for (auto& line : File::readTextLines(filePath))
    {
        line = line.trimmed();
        // TODO: validate URL
        if (line.length() > 0)
            inputTable->appendRow(QStringList() << line << "");
    }
    m_lastDirectory = QDir(filePath).absolutePath();
    m_workspaceWidget->switchToSourcesTab();
}

void MainWindow::importUrls()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Import URLs", m_lastDirectory, "Text files (*.txt);;All files (*.*)");
    importUrlFile(filePath);
    m_recentFiles->addFile(filePath);
}

void MainWindow::importRecentFileUrls(const QString& filePath)
{
    importUrlFile(filePath);
}

void MainWindow::exportResults()
{
    Table* resultsTable = m_workspaceWidget->resultsTable();
    if (resultsTable->rowCount() == 0)
    {
        QMessageBox::information(this, "Export Results", "Nothing to export!");
        return;
    }
    QString filePath(QDir(m_lastDirectory).absoluteFilePath("results.txt"));
    filePath = QFileDialog::getSaveFileName(this, "Export Results", filePath, "Text files (*.txt)");
    QStringList urls;
    for (int i = 0; i < resultsTable->rowCount(); ++i)
    {
        urls << resultsTable->cell(i, 0).toString();
    }
    File::writeTextFile(filePath, urls);
    m_lastDirectory = QDir(filePath).absolutePath();
}

void MainWindow::setStatusMessage(const QString& message)
{
    m_statusBarLabel->setText(message);
}
