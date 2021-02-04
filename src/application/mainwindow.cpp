#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QIODevice>
#include <QHeaderView>
#include <QLabel>
// #include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QModelIndex>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QPair>
#include <QPoint>
#include <QProgressBar>
#include <QPushButton>
#include <QRect>
#include <QSettings>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStringList>
#include <QTextStream>
#include <QTextEdit>
#include <QTimer>
#include <QTreeWidget>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>

#include "applicationstate.h"
#include "config.h"
#include "file.h"
#include "httpclient.h"
#include "mainwindow.h"
#include "table.h"
#include "version.h"
#include "recentfiles.h"
#include "sidebar.h"
#include "proxieswidget.h"


MainWindow::MainWindow ( QWidget* parent ) : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("URL Checker - %1").arg(APP_VERSION));
    resize(800, 600);

    m_columnRatios << 0.5 << 0.2 << 0.2;
    QDir applicationDir(QApplication::applicationDirPath());
    m_settingsFilePath = applicationDir.absoluteFilePath("settings.ini");
    m_lastDirectory = applicationDir.absolutePath(); 
    m_networkManager = new QNetworkAccessManager(this);
    m_httpClient = new HttpClient(this);
    m_pulseTimer = new QTimer(this);
    m_recentFiles = new RecentFiles(5, this);
    m_reply = nullptr;
    m_recentUrlFileActions = QList<QAction*>();

   m_applicationState = new ApplicationState(this);
   m_applicationState->start();

    createActions();
    createMenuBar();
    createToolBar();
    createWidgets();
    createStatusBar();
    createConnections();

    m_httpClient->setTimeout(m_timeoutSpinBox->value());
    m_httpClient->setUserAgent(QString(USER_AGENT));

    if (QFile::exists(m_settingsFilePath))
        loadSettings();
    else
        centerWindow();
    initRecentUrlFiles();

    for (auto& line : File::readTextLines("/mnt/ramdisk/urls.txt"))
    {
        line = line.trimmed();
        // TODO: validate URL
        if (line.length() > 0)
            m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
    }
    m_pulseTimer->start(1 * 1000);
    emit m_applicationState->applicationReady();
}

MainWindow::~MainWindow()
{
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
        if (line.length() > 0)
            m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
    }
    m_lastDirectory = QDir(filePath).absolutePath();
    addToRecentUrlFiles(filePath);
}

void MainWindow::exportResults()
{
    QString filePath(QDir(m_lastDirectory).absoluteFilePath("results.txt"));
    filePath = QFileDialog::getSaveFileName(this, "Export Results", filePath, "Text files (*.txt)");
    QStringList urls;
    int columnCount = 4;
    int rowCount = m_resultsTable->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        urls << m_resultsTable->cell(i, 0).toString();
    }
    if (urls.length() == 0)
    {
        QMessageBox::information(this, "Export Results", "Nothing to export!");
        return;
    }
    File::writeTextFile(filePath, urls);
    m_lastDirectory = QDir(filePath).absolutePath();
}

void MainWindow::removeDuplicates()
{
    QSet<QString> urls;
    QSet<int> duplicateIndexes;
    QString url;
    for (int i = 0; i < m_resultsTable->rowCount(); ++i)
    {
        url = m_resultsTable->cell(i, 0).toString();
        if (urls.contains(url))
            duplicateIndexes.insert(i);
        else
            urls.insert(url);
    }
    for (int i = m_resultsTable->rowCount() - 1; i >= 0; --i)
    {
        if (duplicateIndexes.contains(i))
            m_resultsTable->removeRow(i);
    }
}

void MainWindow::removeSelected()
{
    QSet<int> selectedIndexes = m_resultsTable->selectedRows();
    for (int i = m_resultsTable->rowCount() - 1; i >= 0; --i)
    {
        if (selectedIndexes.contains(i))
            m_resultsTable->removeRow(i);
    }
}

void MainWindow::centerWindow()
{
    QRect fg = this->frameGeometry();
    QPoint screenCenter = qApp->desktop()->availableGeometry().center();
    fg.moveCenter(screenCenter);
    this->move(fg.topLeft());
}

void MainWindow::createActions()
{
    // Sidebar
    m_projectAction = new QAction(QIcon(":assets/icons/network-clouds.png"), "Projects", this);
    m_settingsAction = new QAction(QIcon(":assets/icons/gear.png"), "Settings", this);
    m_proxiesAction = new QAction(QIcon(":assets/icons/mask.png"), "Proxies", this);
    m_helpAction = new QAction(QIcon(":assets/icons/question.png"), "Help", this);


    m_importUrlsAction = new QAction(QIcon(":assets/icons/table-import.png"), "Import URLs", this);
    m_recentUrlFilesMenu = new QMenu("Open Recent URL File", this);
    m_clearRecentUrlFilesAction = new QAction(QIcon(":assets/icons/broom.png"), "Clear List", this);
    m_exportResultsAction = new QAction(QIcon(":assets/icons/table-export.png"), "Export Results", this);
    m_quitAction = new QAction(QIcon(":assets/icons/cross-circle.png"), "Quit", this);
    m_clearTableAction = new QAction(QIcon(":assets/icons/broom.png"), "Clear Results Table", this);
    m_removeDuplicatesAction = new QAction(QIcon(":assets/icons/table-delete-row.png"), "Remove Duplicates", this);
    m_selectAllAction = new QAction(QIcon(":assets/icons/table-select-all.png"), "Select All Rows", this);
    m_invertSelectionAction = new QAction(QIcon(":assets/icons/table.png"), "Invert Selection", this);
    m_removeSelectedAction= new QAction(QIcon(":assets/icons/table-delete-row.png"), "Remove Selected Rows", this);
    m_centerWindowAction = new QAction(QIcon(":assets/icons/zone-resize-actual.png"), "Center Window", this);
    m_aboutAction = new QAction(QIcon(":assets/icons/information.png"), "About", this);
}

void MainWindow::createMenuBar()
{
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_selectionMenu = menuBar()->addMenu(tr("Selection"));
    m_windowMenu = menuBar()->addMenu(tr("Window"));
    m_helpMenu = menuBar()->addMenu(tr("Help"));

    m_fileMenu->addAction(m_importUrlsAction);
    m_fileMenu->addMenu(m_recentUrlFilesMenu);
    m_recentUrlFilesMenu->addAction(m_clearRecentUrlFilesAction);
    m_fileMenu->addAction(m_exportResultsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);

    m_editMenu->addAction(m_clearTableAction);
    m_editMenu->addAction(m_removeDuplicatesAction);

    m_selectionMenu->addAction(m_removeSelectedAction);
    m_selectionMenu->addAction(m_invertSelectionAction);
    m_selectionMenu->addAction(m_removeSelectedAction);

    m_windowMenu->addAction(m_centerWindowAction);

    m_helpMenu->addAction(m_aboutAction);
}


void MainWindow::createToolBar()
{
    m_toolBar = addToolBar("mainToolBar");
    m_toolBar->setObjectName("mainToolBar");
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);

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
    m_mainStackedWidget = new QStackedWidget;

    m_centralLayout = new QHBoxLayout(m_centralWidget);
    m_sideBar = new SideBar;

    m_sideBar->addAction(m_projectAction);
    m_sideBar->addAction(m_settingsAction);
    m_sideBar->addAction(m_proxiesAction);
    m_sideBar->addAction(m_helpAction);

    m_projectPageWidget = new QWidget;
    m_projectPageLayout = new QVBoxLayout(m_projectPageWidget);
    m_settingsPageWidget = new QWidget;
    m_settingsPageLayout = new QVBoxLayout(m_settingsPageWidget);
    m_proxiesPageWidget = new QWidget;
    m_proxiesPageLayout = new QVBoxLayout(m_proxiesPageWidget);
    m_helpPageWidget = new QWidget;
    m_helpPageLayout = new QVBoxLayout(m_helpPageWidget);

    m_centralLayout->addWidget(m_sideBar);
    m_centralLayout->addWidget(m_mainStackedWidget);


    m_mainStackedWidget->addWidget(m_projectPageWidget);
    m_mainStackedWidget->addWidget(m_settingsPageWidget);
    m_mainStackedWidget->addWidget(m_proxiesPageWidget);
    m_mainStackedWidget->addWidget(m_helpPageWidget);

    m_toolsTreeWidget = new QTreeWidget;
    m_toolsTreeWidget->setFixedWidth(200);
    m_toolsTreeWidget->setColumnCount(1);
    m_toolsTreeWidget->setHeaderLabel("Tools");
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem *item = nullptr;
    item = new QTreeWidgetItem(QStringList(QString("Check URL Status")));
    item->setIcon(0, QIcon(":assets/icons/hammer.png"));
    items.append(item);
    item = new QTreeWidgetItem(QStringList(QString("Check Alexa Rank")));
    item->setIcon(0, QIcon(":assets/icons/hammer.png"));
    items.append(item);
    //     for (int i = 0; i < 10; ++i)
//     {
//         auto item = new QTreeWidgetItem(QStringList(QString("item: %1").arg(i)));
//         item->setIcon(0, QIcon(":assets/icons/hammer.png"));
//         items.append(item);
//     }
// //         items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    m_toolsTreeWidget->insertTopLevelItems(0, items);
    connect(m_toolsTreeWidget, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        m_toolsPushButton->setText(QString(" %1").arg(current->text(0)));
    });


    m_bottomLayout = new QHBoxLayout;
    m_resultsTable = new Table(QStringList() << "URL" << "Result" << "Code" << "Status", this);
    m_resultsTable->setColumnRatios(m_columnRatios);
    m_threadsLabel = new QLabel("Threads");
    m_timeoutLabel = new QLabel("Timeout (secs)");
    m_threadsSpinBox = new QSpinBox;
    m_threadsSpinBox->setRange(1, 50);
    m_timeoutSpinBox = new QSpinBox;
    m_timeoutSpinBox->setRange(1, 120);
    m_startPushButton = new QPushButton(QIcon(":assets/icons/control.png"), "Start");
    m_stopPushButton = new QPushButton(QIcon(":assets/icons/control-stop-square.png"), "Stop");
    m_testPushButton = new QPushButton("Test");
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);

    m_proxiesTextEdit = new ProxiesWidget;
    m_proxiesTextEdit->setReadOnly(true);

    auto projectPageHLayout = new QHBoxLayout;
    m_projectPageLayout->addLayout(projectPageHLayout);
    projectPageHLayout->addWidget(m_toolsTreeWidget);
    projectPageHLayout->addWidget( m_resultsTable->tableView() );
    m_bottomLayout->addStretch(0);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    m_bottomLayout->addWidget(m_testPushButton);
    m_projectPageLayout->addLayout(m_bottomLayout);
    m_projectPageLayout->addWidget(m_progressBar);

    m_settingsPageLayout->addWidget(m_threadsLabel);
    m_settingsPageLayout->addWidget(m_threadsSpinBox);
    m_settingsPageLayout->addWidget(m_timeoutLabel);
    m_settingsPageLayout->addWidget(m_timeoutSpinBox);
    m_settingsPageLayout->addStretch(0);

    m_proxiesPageLayout->addWidget(m_proxiesTextEdit);

    setCentralWidget(m_centralWidget);
}

void MainWindow::createStatusBar()
{
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);

    m_toolsPushButton = new QPushButton(QIcon(":assets/icons/hammer.png"), "");
    auto statusBarLabel = new QLabel;
    m_activeThreadsLabel = new QLabel(" Active threads: /");

    connect(m_toolsPushButton, &QPushButton::clicked, [this]{
        if (m_toolsTreeWidget->isVisible())
            m_toolsTreeWidget->setVisible(false);
        else
            m_toolsTreeWidget->setVisible(true);
    });

    m_statusBar->addPermanentWidget(m_toolsPushButton);
    m_statusBar->addPermanentWidget(statusBarLabel, 1);
    m_statusBar->addPermanentWidget(m_activeThreadsLabel);
}

void MainWindow::createConnections()
{
    connect(m_testPushButton, &QPushButton::clicked, [this]{
        m_startPushButton->setEnabled(false);
    });
    connect(m_pulseTimer, &QTimer::timeout, this, &MainWindow::onPulse);

    connect(m_projectAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(0);});
    connect(m_settingsAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(1);});
    connect(m_proxiesAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(2);});
    connect(m_helpAction, &QAction::triggered, [this]{m_mainStackedWidget->setCurrentIndex(3);});


    connect(m_centerWindowAction, &QAction::triggered, this, &MainWindow::centerWindow);
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::close);
    connect(m_aboutAction, &QAction::triggered, [&] {QMessageBox::about(this,
        "About " APP_TITLE,

        "<p>Fugue icons are provided by <a href='http://p.yusukekamiyamane.com/'>Yusuke"
        "Kamiyamane</a>"
    );});
    connect(m_importUrlsAction, &QAction::triggered, this, &MainWindow::importUrls);
    connect(m_exportResultsAction, &QAction::triggered, this, &MainWindow::exportResults);
    connect(m_clearTableAction, &QAction::triggered, [this] {m_resultsTable->removeAllRows();});
    connect(m_selectAllAction, &QAction::triggered, [this] {m_resultsTable->selectAll();});
    connect(m_invertSelectionAction, &QAction::triggered, [this] {m_resultsTable->invertSelection();});
    connect(m_removeDuplicatesAction, &QAction::triggered, this, &MainWindow::removeDuplicates);
    connect(m_removeSelectedAction, &QAction::triggered, this, &MainWindow::removeSelected);
    connect(m_startPushButton, &QPushButton::clicked, this, &MainWindow::startChecking);
    connect(m_stopPushButton, &QPushButton::clicked, this, &MainWindow::stopChecking);
    connect(m_resultsTable, &Table::doubleClicked, [this] (const QModelIndex &modelIndex) {
        QDesktopServices::openUrl(QUrl(m_resultsTable->cell(modelIndex.row(), 0).toString()));
    });
    connect(m_recentFiles, &RecentFiles::filePathSelected, this, &MainWindow::onSelectedRecentUrlFile);

    connect(m_httpClient, &HttpClient::replyFinished, this, &MainWindow::urlChecked);

    connect(m_applicationState, &ApplicationState::applicationStarted, [this]{
        m_statusBar->showMessage("Starting ...");
        qDebug() << "Application starting ...";
    });
    connect(m_applicationState, &ApplicationState::applicationReady, [this]{
        m_statusBar->showMessage("Ready.");
        m_startPushButton->setEnabled(true);
        m_stopPushButton->setEnabled(false);
        qDebug() << "Application idling ...";
    });
    connect(m_applicationState, &ApplicationState::applicationExit, [this]{
        m_statusBar->showMessage("Exiting ...");
        m_startPushButton->setEnabled(false);
        qDebug() << "Application exiting ...";
    });
    connect(m_applicationState, &ApplicationState::jobStarted, [this]{
        m_statusBar->showMessage("Working ...");
        m_startPushButton->setEnabled(false);
        m_stopPushButton->setEnabled(true);
        qDebug() << "Job started";
    });
    connect(m_applicationState, &ApplicationState::jobStopped, [this]{
        m_statusBar->showMessage("Stopping ...");
        m_startPushButton->setEnabled(false);
        m_stopPushButton->setEnabled(false);
        qDebug() << "Job stopped";
    });
    connect(m_applicationState, &ApplicationState::jobFinished, [this]{
//         m_statusBar->showMessage("Stopping ...");
        m_startPushButton->setEnabled(false);
        m_stopPushButton->setEnabled(false);
        qDebug() << "Job finished";
    });
    
}

void MainWindow::saveSettings()
{
    QSettings settings(m_settingsFilePath, QSettings::IniFormat);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("threadsCount", m_threadsSpinBox->value());
    settings.setValue("timeout", m_timeoutSpinBox->value());
    settings.setValue("lastDirectory", m_lastDirectory);
}

void MainWindow::loadSettings()
{
    if (QFile::exists(m_settingsFilePath))
    {
        QSettings settings(m_settingsFilePath, QSettings::IniFormat);
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("windowState").toByteArray());
        m_threadsSpinBox->setValue(settings.value("threadsCount", 1).toInt());
        m_timeoutSpinBox->setValue(settings.value("timeout", 20).toInt());
        m_lastDirectory = settings.value("lastDirectory", m_lastDirectory).toString();
    }
}

// Events
void MainWindow::closeEvent(QCloseEvent* event)
{
   emit m_applicationState->applicationExiting();
    saveSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    m_resultsTable->resizeColumns();
    QMainWindow::resizeEvent(event);
}

// Slots
void MainWindow::startChecking()
{
    emit m_applicationState->jobStarting();
    if (m_resultsTable->rowCount() > 0)
    {
        m_progressBar->setValue(0);
        m_currentRowIndex = 0;
        m_running = true;
        m_httpClient->setTimeout(m_timeoutSpinBox->value());
        m_httpClient->head(m_resultsTable->cell(m_currentRowIndex, 0).toString());
    }
}

void MainWindow::stopChecking()
{
    emit m_applicationState->jobStopping();
    m_running = false;
}

void MainWindow::urlChecked(int statusCode, const QString& statusText, const QString& text)
{
    updateResultsRow(m_currentRowIndex, statusCode, statusText);
    int currentProgress = static_cast<int>(static_cast<double>(m_currentRowIndex) / m_resultsTable->rowCount() * 100);
    m_progressBar->setValue(currentProgress);
    if (!m_running || m_currentRowIndex >= m_resultsTable->rowCount())
    {
        emit m_applicationState->jobFinishing();
        return;
    }
    ++m_currentRowIndex;
    m_httpClient->head(m_resultsTable->cell(m_currentRowIndex, 0).toString());
}

void MainWindow::initRecentUrlFiles()
{
    for (QAction *action : m_recentFiles->actions())
    {
        m_recentUrlFilesMenu->addAction(action);
    }
}

void MainWindow::addToRecentUrlFiles(const QString& filePath)
{
    m_recentUrlFileActions.append(new QAction(filePath, this));
}

void MainWindow::updateResultsRow(int rowIndex, const QVariant& statusCode, const QVariant& statusText)
{
    m_resultsTable->setCell(rowIndex, 2, QVariant(statusCode));
    m_resultsTable->setCell(rowIndex, 3, QVariant(statusText));
    int _statusCode = statusCode.toInt();
    if (_statusCode >= 200 && _statusCode < 300)
        m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkGreen));
    else if (_statusCode >= 300 && _statusCode < 400)
        m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkBlue));
    else if (_statusCode >= 400)
        m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkRed));
    else
        m_resultsTable->setRowColor(rowIndex, QColor(Qt::darkGray), QColor(Qt::darkYellow));
}

void MainWindow::onPulse()
{
    //
}

void MainWindow::onSelectedRecentUrlFile(const QString& filePath)
{
    qDebug() << "open recent file" << filePath;
}
