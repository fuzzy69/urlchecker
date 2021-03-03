#include "workspacewidget.h"

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

#include "../common/table.h"
#include "../core/toolswidget.h"
#include "../core/tools.h"


WorkspaceWidget::WorkspaceWidget(QWidget* parent) : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_topLayout = new QHBoxLayout;
    m_bottomLayout = new QHBoxLayout;

    m_toolsWidget = new ToolsWidget(Tool(Tools::CHECK_URL_STATUS, QIcon(":assets/icons/chain.png"), QString("Check URL Status")));
    m_toolsWidget->addTool(Tool(Tools::CHECK_ALEXA_RANK, QIcon(":assets/icons/alexa.png"), QString("Check Alexa Rank")));
    m_toolsWidget->addTool(Tool(Tools::SCRAPE_PROXIES, QIcon(":assets/icons/mask.png"), QString("Scrape Proxies")));
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
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);

    m_tabWidget->addTab(m_inputTable->tableView(), QString("Input Data"));
    m_tabWidget->addTab(m_resultsTable->tableView(), QString("Results"));
    m_topLayout->addWidget(m_toolsWidget);
    m_topLayout->addWidget(m_tabWidget);
    m_bottomLayout->addStretch(0);
    m_bottomLayout->addWidget(m_startPushButton);
    m_bottomLayout->addWidget(m_stopPushButton);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_bottomLayout);
    m_mainLayout->addWidget(m_progressBar);

    connect(m_resultsTable, &Table::doubleClicked, [this] (const QModelIndex &modelIndex) {
        QDesktopServices::openUrl(QUrl(m_resultsTable->cell(modelIndex.row(), 0).toString()));
    });
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
