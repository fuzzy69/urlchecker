#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

#include "my/browserutils.h"
#include "my/file.h"
#include "my/proxymanager.h"
#include "my/proxyutils.h"
#include "my/useragents.h"

#include "config.h"
#include "constants.h"
#include "core/applicationbridge.h"
#include "core/applicationstatemachine.h"
#include "core/misc.h"
#include "core/recentfiles.h"
#include "core/settings.h"
#include "core/table.h"
#include "core/thread.h"
#include "icons.h"
#include "mainwindow.h"
#include "texts.h"
#include "tools/tool.h"
#include "version.h"
#include "widgets/filesystemwidget.h"
#include "widgets/helpwidget.h"
#include "widgets/proxieswidget.h"
#include "widgets/settingswidget/settingswidget.h"
#include "widgets/sidebarwidget.h"
#include "widgets/tableswidget.h"
#include "widgets/toolswidget.h"
#include "widgets/useragentswidget.h"
#include "widgets/workspacewidget.h"

#include "actions.h"
#include "core/actionsmanager.h"
#include "core/workermanager.h"

using my::browser::UserAgentsManager;
using my::data::USER_AGENTS_TEXT;
using my::filesystem::File;
using my::network::ProxyManager;

MainWindow::MainWindow(QWidget* parent)
    : MainWindowBase(parent)
{
    setWindowTitle(QStringLiteral("%1 - %2").arg(QStringLiteral(APPLICATION_TITLE)).arg(QStringLiteral(APPLICATION_VERSION)));

    QDir applicationDir(QApplication::applicationDirPath());
    applicationDir.mkdir("data");
    //
    m_applicationStateMachine = new ApplicationStateMachine(this);
    m_pulseTimer = new QTimer(this);
    m_recentFiles = new RecentFiles(MAX_RECENT_FILES, this);

    createActions();
    createMenuBar();
    createToolBar();
    createWidgets();
    createStatusBar();
    createConnections();

    initSettings(applicationDir);
    initUserAgents(applicationDir);
    initProxies(applicationDir);

    // Init recent files
    for (QAction* action : m_recentFiles->actions()) {
        m_recentUrlFilesMenu->addAction(action);
    }

    m_applicationStateMachine->start();
    m_pulseTimer->start(1 * MILLIS_IN_SECOND);
    QTimer::singleShot(3 * MILLIS_IN_SECOND, [this]() {
        Q_EMIT m_applicationStateMachine->applicationReady();
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    // Groups
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_selectionMenu = menuBar()->addMenu(tr("Selection"));
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_filterMenu = menuBar()->addMenu(tr("Filter"));
    m_windowMenu = menuBar()->addMenu(tr("Window"));
    m_helpMenu = menuBar()->addMenu(tr("Help"));
    // File menu
    m_fileMenu->addAction(ActionsManager::instance().action("importUrls"));
    m_recentUrlFilesMenu = new QMenu(tr("Open Recent URL File"), this);
    m_fileMenu->addMenu(m_recentUrlFilesMenu);
    //    m_recentUrlFilesMenu->addAction(m_clearRecentUrlFilesAction);
    m_recentUrlFilesMenu->addAction(ActionsManager::instance().action("clearRecentUrlFiles"));
    m_recentUrlFilesMenu->addSeparator();
    m_fileMenu->addAction(ActionsManager::instance().action("exportResults"));
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(ActionsManager::instance().action("quit"));
    // Selection menu
    m_selectionMenu->addAction(ActionsManager::instance().action("selectAllRows"));
    m_selectionMenu->addAction(ActionsManager::instance().action("invertRowsSelection"));
    // Edit menu
    m_editMenu->addAction(ActionsManager::instance().action("removeSelectedRows"));
    m_editMenu->addAction(ActionsManager::instance().action("removAllRows"));
    m_editMenu->addAction(ActionsManager::instance().action("removeDuplicates"));
    // Filter menu
    // Window menu
    m_windowMenu->addAction(ActionsManager::instance().action("centerWindow"));
    // Help menu
    m_helpMenu->addAction(ActionsManager::instance().action("about"));
}

void MainWindow::createToolBar()
{
    // Toolbar
    m_toolBar = addToolBar("mainToolBar");
    m_toolBar->setObjectName("mainToolBar");
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
    // Buttons
    m_toolBar->addAction(ActionsManager::instance().action("importUrls"));
    m_toolBar->addAction(ActionsManager::instance().action("exportResults"));
    m_toolBar->addSeparator();
    m_toolBar->addAction(ActionsManager::instance().action("removeSelectedRows"));
    m_toolBar->addAction(ActionsManager::instance().action("removeDuplicates"));
    m_toolBar->addAction(ActionsManager::instance().action("removAllRows"));
    m_toolBar->addSeparator();
    m_toolBar->addAction(ActionsManager::instance().action("selectAllRows"));
    m_toolBar->addAction(ActionsManager::instance().action("invertRowsSelection"));
    m_toolBar->addSeparator();
    m_toolBar->addAction(ActionsManager::instance().action("quit"));
}

void MainWindow::createWidgets()
{
    // Sidebar
    m_sideBarWidget = new SideBarWidget;
    m_sideBarWidget->addAction(ActionsManager::instance().action("workspace"), true);
    m_sideBarWidget->addAction(ActionsManager::instance().action("settings"));
    m_sideBarWidget->addAction(ActionsManager::instance().action("userAgents"));
    m_sideBarWidget->addAction(ActionsManager::instance().action("proxies"));
    m_sideBarWidget->addAction(ActionsManager::instance().action("help"));
    // Main widgets
    m_mainStackedWidget = new QStackedWidget;
    m_workspaceWidget = new WorkspaceWidget;
    // FIXME: Application sometimes crashes on exit, possible ownership issue with SettingsWidget?
    m_settingsWidget = new SettingsWidget;
    m_userAgentsWidget = new UserAgentsWidget;
    m_proxiesWidget = new ProxiesWidget;
    m_helpWidget = new HelpWidget;

    m_mainStackedWidget->addWidget(m_workspaceWidget);
    m_mainStackedWidget->addWidget(m_settingsWidget);
    m_mainStackedWidget->addWidget(m_userAgentsWidget);
    m_mainStackedWidget->addWidget(m_proxiesWidget);
    m_mainStackedWidget->addWidget(m_helpWidget);
    // Central widget
    m_centralWidget = new QWidget;
    m_centralLayout = new QHBoxLayout(m_centralWidget);
    m_centralLayout->addWidget(m_sideBarWidget);
    m_centralLayout->addWidget(m_mainStackedWidget);

    setCentralWidget(m_centralWidget);

    ApplicationBridge::instance().setSettingsWidget(m_settingsWidget);
    ApplicationBridge::instance().setProxiesWidget(m_proxiesWidget);
}

void MainWindow::createStatusBar()
{
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);

    m_toolsPushButton = new QPushButton(QIcon(ICON_HAMMER), QStringLiteral(" Tools"));
    m_logPushButton = new QPushButton(QIcon(ICON_DOCUMENT_LIST), QStringLiteral(" Log"));
    m_statusBarLabel = new QLabel;
    m_activeThreadsLabel = new QLabel(tr(TEXT_ACTIVE_THREADS));

    m_statusBar->addPermanentWidget(m_toolsPushButton);
    m_statusBar->addPermanentWidget(m_logPushButton);
    m_statusBar->addPermanentWidget(m_statusBarLabel, 1);
    m_statusBar->addPermanentWidget(m_activeThreadsLabel);

    ApplicationBridge::instance().setStatusBar(m_statusBar);
}

void MainWindow::createConnections()
{
    // File menu
    connect(ActionsManager::instance().action(ACTION_IMPORT_URLS), &QAction::triggered, this, &MainWindow::importUrls);
    connect(ActionsManager::instance().action(ACTION_EXPORT_RESULTS), &QAction::triggered, this, &MainWindow::exportResults);
    connect(m_recentFiles, &RecentFiles::filePathSelected, this, &MainWindow::importRecentFileUrls);
    connect(ActionsManager::instance().action(ACTION_CLEAR_RECENT_URL_FILES), &QAction::triggered, [this] {
        m_recentFiles->clear();
    });
    connect(ActionsManager::instance().action(ACTION_QUIT), &QAction::triggered, this, &MainWindow::close);

    // Window
    connect(ActionsManager::instance().action(ACTION_CENTER_WINDOW), &QAction::triggered, this, &MainWindow::centerWindow);

    //Help menu
    connect(ActionsManager::instance().action(ACTION_ABOUT), &QAction::triggered, [&] { QMessageBox::about(this,
                                                                                            QString("About %1").arg(APPLICATION_TITLE),
                                                                                            QString(
                                                                                                "<h3>%1 %2</h3><br/>"
                                                                                                "%3<br/>"
                                                                                                "Fugue icons are provided by <a href='http://p.yusukekamiyamane.com/'>Yusuke"
                                                                                                "Kamiyamane</a>")
                                                                                                .arg(APPLICATION_TITLE, APPLICATION_VERSION, APPLICATION_DESCRIPTION)); });

    // Sidebar
    connect(m_sideBarWidget, &SideBarWidget::currentActionIndexChanged, m_mainStackedWidget, &QStackedWidget::setCurrentIndex);

    // Misc
    connect(m_pulseTimer, &QTimer::timeout, this, &MainWindow::onPulse);

    // Workspace widget
    connect(m_workspaceWidget->workerManager(), &WorkerManager::jobStarted, m_applicationStateMachine, &ApplicationStateMachine::jobStart);
    connect(m_workspaceWidget->workerManager(), &WorkerManager::jobStopped, m_applicationStateMachine, &ApplicationStateMachine::jobStop);

    // Table actions
    connect(ActionsManager::instance().action(ACTION_SELECT_ALL_ROWS), &QAction::triggered, [this] { m_workspaceWidget->tablesWidget()->focusedTable()->selectAll(); });
    connect(ActionsManager::instance().action(ACTION_INVERT_ROWS_SELECTION), &QAction::triggered, [this] { m_workspaceWidget->tablesWidget()->focusedTable()->invertSelection(); });
    connect(ActionsManager::instance().action(ACTION_REMOVE_SELECTED_ROWS), &QAction::triggered, [this] { m_workspaceWidget->tablesWidget()->focusedTable()->removeSelected(); });
    connect(ActionsManager::instance().action(ACTION_REMOVE_DUPLICATE_ROWS), &QAction::triggered, [this] { m_workspaceWidget->tablesWidget()->focusedTable()->removeDuplicates(); });
    connect(ActionsManager::instance().action(ACTION_REMOVE_ALL_ROWS), &QAction::triggered, [this] { m_workspaceWidget->tablesWidget()->focusedTable()->removeAllRows(); });

    // Tools
    connect(m_workspaceWidget->toolsWidget(), &ToolsWidget::toolSettingsRequested, [this](const Tool& tool) {
        if (m_settingsWidget->setCurrentSettingsPage(tool.name())) {
            m_sideBarWidget->setCurrentAction(1);
        }
    });

    // Filesystem
    connect(m_workspaceWidget->filesystemWidget(), &FilesystemWidget::urlFileDoubleClicked, [this](const QString& filePath) {
        importUrlFile(filePath);
        m_recentFiles->addFile(filePath);
    });

    // Statusbar
    connect(m_toolsPushButton, &QPushButton::clicked, [this]() {
        m_workspaceWidget->toggleSideTabWidget();
    });
    connect(m_logPushButton, &QPushButton::clicked, [this]() {
        m_workspaceWidget->toggleLogWidget();
    });

    // Application states
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationStarted, [this]() {
        m_workspaceWidget->onApplicationStart();
    });
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationIdling, [this]() {
        m_workspaceWidget->onApplicationReady();
        m_statusBarLabel->setText("Ready.");
    });
    connect(m_applicationStateMachine, &ApplicationStateMachine::applicationExiting, [this]() {
        m_workspaceWidget->onApplicationExit();
    });
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobStarted, [this]() {
        m_workspaceWidget->onJobStart();
    });
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobStopping, [this]() {
        m_workspaceWidget->onJobStop();
    });
    connect(m_applicationStateMachine, &ApplicationStateMachine::jobFinished, [this]() {
        m_workspaceWidget->onJobDone();
    });
}

void MainWindow::loadSettings()
{
    Settings::instance().load();
    restoreGeometry(QByteArray::fromHex(Settings::instance().value(QStringLiteral(TEXT_GEOMETRY)).toByteArray()));
    restoreState(QByteArray::fromHex(Settings::instance().value(QStringLiteral(TEXT_WINDOW_STATE)).toByteArray()));
    m_lastDirectory = Settings::instance().value(QStringLiteral(TEXT_LAST_DIRECTORY)).toString();
}

void MainWindow::saveSettings()
{
    Settings::instance().setValue(QStringLiteral(TEXT_GEOMETRY), saveGeometry().toHex());
    Settings::instance().setValue(QStringLiteral(TEXT_WINDOW_STATE), saveState().toHex());
    Settings::instance().setValue(QStringLiteral(TEXT_LAST_DIRECTORY), m_lastDirectory);
    Settings::instance().save();
}

// Events
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_applicationStateMachine->currentState() == ApplicationState::JOB_RUNNING) {
        auto reply = QMessageBox::question(this, QStringLiteral("Close Confirmation"), QStringLiteral("Some jobs are stil running, close anyway?"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    // TODO: Stop remaining running workers/threads
    Q_EMIT m_applicationStateMachine->applicationExiting();
    saveSettings();
    // Save user agents to file
    File::writeTextFile(Settings::instance().value(QStringLiteral(USER_AGENTS_FILE)).toString(), m_userAgentsWidget->toPlainText());
    // Save proxies to file
    File::writeTextFile(Settings::instance().value(QStringLiteral(PROXIES_FILE)).toString(), m_proxiesWidget->toPlainText());

    QMainWindow::closeEvent(event);
}

// Slots
void MainWindow::onPulse()
{
    m_activeThreadsLabel->setText(QString(" Active threads: %1").arg(Thread::count()));
    if (m_applicationStateMachine->currentState() == ApplicationState::JOB_RUNNING && Thread::count() == 0)
        emit m_applicationStateMachine->jobDone();
}

void MainWindow::importUrlFile(const QString& filePath)
{
    if (!QFile::exists(filePath))
        return;
    Table* inputTable = m_workspaceWidget->tablesWidget()->inputTable();
    if (inputTable->rowCount() != 0) {
        QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Import URLs"), QStringLiteral("Input table not empty! Append to existing rows or"
                                                                                                    " replace current rows?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        messageBox.setButtonText(QMessageBox::Yes, "Append");
        messageBox.setButtonText(QMessageBox::No, "Replace");
        int pushedButton = messageBox.exec();
        if (pushedButton == QMessageBox::Cancel)
            return;
        if (pushedButton == QMessageBox::No)
            inputTable->removeAllRows();
    }
    for (auto& line : File::readTextLines(filePath)) {
        line = line.trimmed();
        // TODO: validate URL
        if (line.length() > 0)
            inputTable->appendRow(QStringList() << line << "");
    }
    m_lastDirectory = QDir(filePath).absolutePath();
    m_workspaceWidget->tablesWidget()->switchToSourcesTab();
}

void MainWindow::importUrls()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("Import URLs"), m_lastDirectory, QStringLiteral("Text files (*.txt);;All files (*.*)"));
    importUrlFile(filePath);
    m_recentFiles->addFile(filePath);
}

void MainWindow::importRecentFileUrls(const QString& filePath)
{
    importUrlFile(filePath);
}

void MainWindow::exportResults()
{
    Table* resultsTable = m_workspaceWidget->tablesWidget()->resultsTable();
    if (resultsTable->rowCount() == 0) {
        QMessageBox::information(this, QStringLiteral("Export Results"), QStringLiteral("Nothing to export!"));
        return;
    }
    QString filePath(QDir(m_lastDirectory).absoluteFilePath(""));
    filePath = QFileDialog::getSaveFileName(this, QStringLiteral("Export Results"), filePath, QStringLiteral("Text files (*.txt);;CSV files (*.csv)"));
    if (filePath.length() > 0) {
        QFileInfo fileInfo(filePath);
        QStringList lines;
        if (fileInfo.suffix().toLower() == QStringLiteral("csv")) {
            lines << resultsTable->columnNames().join(QStringLiteral(","));
            for (int i = 0; i < resultsTable->rowCount(); ++i) {
                QStringList cells;
                for (int j = 0; j < resultsTable->columnCount(); ++j) {
                    cells.append(resultsTable->cell(i, j).toString());
                }
                lines << cells.join(QStringLiteral(","));
            }
        } else if (fileInfo.suffix().toLower() == QStringLiteral("txt")) {
            for (int i = 0; i < resultsTable->rowCount(); ++i) {
                lines << resultsTable->cell(i, 0).toString();
            }
        } else {
            QMessageBox::warning(this, QStringLiteral("Export Results"), QStringLiteral("Unsupported file type!"));
            return;
        }
        File::writeTextFile(filePath, lines);
        m_lastDirectory = QDir(filePath).absolutePath();
    }
}

void MainWindow::initSettings(const QDir& applicationDir)
{
    m_settingsFilePath = applicationDir.filePath(QStringLiteral(SETTINGS_FILE));
    Settings::instance().setFilePath(m_settingsFilePath);
    Settings::instance().setValue(QStringLiteral(TEXT_THREADS), QVariant(THREADS));
    Settings::instance().setValue(QStringLiteral(TEXT_TIMEOUT), QVariant(HTTP_REQUEST_TIMEOUT_));
    Settings::instance().setValue(QStringLiteral(TEXT_USE_PROXIES), QVariant(USE_PROXIES));
    Settings::instance().setValue(QStringLiteral(TEXT_LAST_DIRECTORY), QVariant(applicationDir.absolutePath()));
    Settings::instance().setValue(QStringLiteral(TEXT_PROXIES_FILE), QVariant(applicationDir.filePath(QStringLiteral(PROXIES_FILE))));
    Settings::instance().setValue(QStringLiteral(TEXT_USER_AGENTS_FILE),
        QVariant(applicationDir.filePath(QStringLiteral(USER_AGENTS_FILE))));
    //
    if (QFile::exists(m_settingsFilePath)) {
        loadSettings();
    } else {
        resize(800, 600);
        centerWindow();
    }
}

void MainWindow::initUserAgents(const QDir& applicationDir)
{
    QString userAgentsFilePath = applicationDir.filePath(QStringLiteral(USER_AGENTS_FILE));
    if (!QFile::exists(userAgentsFilePath)) {
        File::writeTextFile(userAgentsFilePath, QString(USER_AGENTS_TEXT));
    }
    for (auto& line : File::readTextLines(userAgentsFilePath)) {
        UserAgentsManager<QString>::instance().add_user_agent(line.trimmed());
        m_userAgentsWidget->append(line);
    }
    UserAgentsManager<QString>::instance().set_default_user_agent(USER_AGENT);
}

void MainWindow::initProxies(const QDir& applicationDir)
{
    QString proxiesFilePath = applicationDir.filePath(QStringLiteral(PROXIES_FILE));
    if (!QFile::exists(proxiesFilePath)) {
        File::writeTextFile(proxiesFilePath, QStringLiteral(""));
    }
    for (auto& proxy : my::network::loadProxiesFromFile(proxiesFilePath)) {
        ProxyManager::instance().add_proxy(proxy);
        m_proxiesWidget->append(QString::fromStdString(proxy));
    }
}
