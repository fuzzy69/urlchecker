#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
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
#include <QPoint>
#include <QProgressBar>
#include <QPushButton>
#include <QRect>
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
#include "mainwindow.h"
#include "table.h"

MainWindow::MainWindow ( QWidget* parent ) : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("URL Checker"));
    resize(800, 600);

    m_columnRatios << 0.5 << 0.2 << 0.1;
    createActions();
    createMenuBar();
    createToolBar();
    createWidgets();
    createStatusBar();
    createConnections();
    centerWindow();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::importUrls()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Import URLs", "/", "Text files (*.txt);;All files (*.*)");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Failed to open URLs file", file.errorString());
        return;
    }
    QTextStream textStream(&file);
    while (!textStream.atEnd())
    {
        QString line = textStream.readLine().trimmed();
        // TODO: validate URL
        if (line.length() > 0)
        {
            m_resultsTable->appendRow(QStringList() << line << "" << "" << "");
//             m_resultsModel->appendRow(
//                 QList<QStandardItem*>()
//                 << new QStandardItem(line)
//                 << new QStandardItem("")
//                 << new QStandardItem("")
//                 << new QStandardItem("")
//             );
        }
    }
}

void MainWindow::exportResults()
{
    
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
    connect(m_clearTableAction, &QAction::triggered, [this] {m_resultsTable->removeAllRows();});
    connect(m_selectAllAction, &QAction::triggered, [this] {m_resultsTable->selectAll();});
    connect(m_invertSelectionAction, &QAction::triggered, [this] {m_resultsTable->invertSelection();});
}

// Events
void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "close event";
    QMainWindow::closeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    qDebug() << "resize event";
    m_resultsTable->resizeColumns();
    QMainWindow::resizeEvent(event);
}
