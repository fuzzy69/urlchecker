#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QDebug>
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
#include <QStatusBar>
#include <QStringList>
#include <QTextStream>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include "config.h"
#include "file.h"
#include "mainwindow.h"
#include "table.h"
#include "version.h"

MainWindow::MainWindow ( QWidget* parent ) : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("URL Checker - %1").arg(APP_VERSION));
    resize(800, 600);

    m_columnRatios << 0.5 << 0.2 << 0.2;
//     QString applicationDirPath = QApplication::applicationDirPath();
    QDir applicationDir(QApplication::applicationDirPath());
//     m_settingsFilePath = applicationDirPath + "settings.ini";
    m_settingsFilePath = applicationDir.absoluteFilePath("settings.ini");
    m_lastDirectory = applicationDir.absolutePath(); 
//     qDebug() << m_settingsFilePath;
    m_networkManager = new QNetworkAccessManager(this);
    createActions();
    createMenuBar();
    createToolBar();
    createWidgets();
    createStatusBar();
    createConnections();
    if (QFile::exists(m_settingsFilePath))
        loadSettings();
    else
        centerWindow();

    for (auto& line : File::readTextLines("/mnt/ramdisk/urls.txt"))
    {
        line = line.trimmed();
        // TODO: validate URL
        if (line.length() > 0)
            m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
    }
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
    m_importUrlsAction = new QAction(QIcon(":assets/icons/table-import.png"), "Import URLs", this);
    m_exportResultsAction= new QAction(QIcon(":assets/icons/table-export.png"), "Export Results", this);
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
    m_mainWidget = new QWidget;
    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_bottomLayout = new QHBoxLayout;
//     m_resultsTableView = new QTableView;
    m_resultsTable = new Table(QStringList() << "URL" << "Result" << "Code" << "Status", this);
//     m_resultsModel = new QStandardItemModel;
//     m_resultsModel->setHorizontalHeaderLabels(QStringList() << "URL" << "Result" << "Code" << "Status");
    m_resultsTable->setColumnRatios(m_columnRatios);
//     m_resultsTableView->horizontalHeader()->setStretchLastSection(true);
//     m_resultsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     m_resultsTableView->setModel(m_resultsModel);
    m_threadsLabel = new QLabel("Threads");
    m_timeoutLabel = new QLabel("Timeout (secs)");
    m_threadsSpinBox = new QSpinBox;
    m_threadsSpinBox->setRange(1, 50);
    m_timeoutSpinBox = new QSpinBox;
    m_timeoutSpinBox->setRange(1, 120);
    m_startPushButton = new QPushButton(QIcon(":assets/icons/control.png"), "Start");
    m_stopPushButton = new QPushButton(QIcon(":assets/icons/control-stop-square.png"), "Stop");
    m_progressBar = new QProgressBar;

    m_mainLayout->addWidget( m_resultsTable->tableView() );
    m_bottomLayout->addWidget(m_threadsLabel);
    m_bottomLayout->addWidget(m_threadsSpinBox);
    m_bottomLayout->addWidget(m_timeoutLabel);
    m_bottomLayout->addWidget(m_timeoutSpinBox);
    m_bottomLayout->addStretch(0);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    m_mainLayout->addLayout(m_bottomLayout);
    m_mainLayout->addWidget(m_progressBar);
    setCentralWidget(m_mainWidget);
}

void MainWindow::createStatusBar()
{
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_activeThreadsLabel = new QLabel(" Active threads: /");
    m_statusBar->showMessage("Ready.");
    m_statusBar->addPermanentWidget(m_activeThreadsLabel);
}

void MainWindow::createConnections()
{
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
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::urlChecked);
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
    m_currentRowIndex = 0;
    m_running = true;
    QNetworkRequest request(m_resultsTable->cell(m_currentRowIndex, 0).toString());
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:70.0) Gecko/20100101 Firefox/70.0");
//     request.setAttribute(QNetworkRequest::User, QVariant(0));
    QNetworkReply *reply = m_networkManager->head(request);
}

void MainWindow::stopChecking()
{
    m_running = false;
}

void MainWindow::urlChecked(QNetworkReply* reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString statusText = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    qDebug() << reply;
    qDebug() << m_currentRowIndex;
    qDebug() << reply->url();
    m_resultsTable->setCell(m_currentRowIndex, 2, QVariant(statusCode));
    m_resultsTable->setCell(m_currentRowIndex, 3, QVariant(statusText));
    reply->deleteLater();
    if (!m_running || m_currentRowIndex >= m_resultsTable->rowCount())
    {
//         disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::urlChecked);
        return;
    }
    ++m_currentRowIndex;
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:70.0) Gecko/20100101 Firefox/70.0");
    request.setUrl(m_resultsTable->cell(m_currentRowIndex, 0).toString());
//     request.setAttribute(QNetworkRequest::User, QVariant(m_currentRowIndex));
    m_networkManager->head(request);
}
