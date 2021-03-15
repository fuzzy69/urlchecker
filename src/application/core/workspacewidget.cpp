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
#include <QTableView>
#include <QTabWidget>
#include <QUrl>
#include <QDesktopServices>

#include "../icons.h"
#include "../common/table.h"
#include "../core/toolswidget.h"
#include "../core/tools.h"
#include "workspacewidget.h"


WorkspaceWidget::WorkspaceWidget(QWidget* parent) : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_topLayout = new QHBoxLayout;
    m_bottomLayout = new QHBoxLayout;

    m_toolsWidget = new ToolsWidget(Tool(Tools::CHECK_URL_STATUS, QIcon(ICON_CHAIN), QString("Check URL Status")));
    m_toolsWidget->addTool(Tool(Tools::CHECK_ALEXA_RANK, QIcon(ICON_ALEXA), QString("Check Alexa Rank")));
    m_toolsWidget->addTool(Tool(Tools::SCRAPE_PROXIES, QIcon(ICON_MASK), QString("Scrape Proxies")));
//     connect(m_toolsWidget, &ToolsWidget::toolSelected, [this](const Tool &tool){
//         m_toolsPushButton->setIcon(tool.icon());
//         m_toolsPushButton->setText(tool.name());
//     });
    m_tabWidget = new QTabWidget;
    m_inputTable = new Table(QStringList() << "URL" << "Status", this);
    m_inputTable->setColumnRatios(QList<float>() << 0.8);
    m_resultsTable = new Table(QStringList() << "URL" << "Result" << "Code" << "Status", this);
    m_resultsTable->setColumnRatios(QList<float>() << 0.5 << 0.2 << 0.2);
    m_startPushButton = new QPushButton(QIcon(":assets/icons/control.png"), "Start");
    m_stopPushButton = new QPushButton(QIcon(":assets/icons/control-stop-square.png"), "Stop");
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
    connect(m_testPushButton, &QPushButton::clicked, this, &WorkspaceWidget::test);
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

void WorkspaceWidget::updateResultsRow(const QMap<QString, QVariant>& resultData)
{
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    //     updateResultsRow(resultData["rowId"].toInt(), resultData["result"].toString(), resultData["status"].toInt(), resultData["message"].toString());
    m_resultsTable->setCell(rowIndex, 1, QVariant(result));
    m_resultsTable->setCell(rowIndex, 2, QVariant(result));
// //     m_resultsTable->setCell(rowIndex, 3, QVariant(statusText));
// //     int _statusCode = statusCode.toInt();
// //     if (_statusCode >= 200 && _statusCode < 300)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkGreen));
// //     else if (_statusCode >= 300 && _statusCode < 400)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkBlue));
// //     else if (_statusCode >= 400)
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::white), QColor(Qt::darkRed));
// //     else
// //         m_resultsTable->setRowColor(rowIndex, QColor(Qt::darkGray), QColor(Qt::darkYellow));
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
