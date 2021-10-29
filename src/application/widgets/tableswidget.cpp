#include "tableswidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QInputDialog>
#include <QTabWidget>
#include <QTableView>
#include <QUrl>
#include <QVBoxLayout>

#include "../actions.h"
#include "../core/actionsmanager.h"
#include "../core/table.h"
#include "../icons.h"
#include "../texts.h"

TablesWidget::TablesWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setContentsMargins(0, 0, 0, 0);
    m_inputTable = new Table(QStringList() << QStringLiteral("URL") << QStringLiteral("Status"), this);
    m_inputTable->setColumnRatios(QList<float>() << 0.8f);
    m_resultsTable = new Table(QStringList() << QStringLiteral("Result"), this);
    m_tabWidget->addTab(m_inputTable->tableView(), QIcon(QStringLiteral(ICON_DOCUMENT_LIST)),
        QStringLiteral("Source URLs"));
    m_tabWidget->addTab(m_resultsTable->tableView(), QIcon(QStringLiteral(ICON_ODATA)), QStringLiteral("Results"));
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_tabWidget);
    // Connections
    connect(m_inputTable, &Table::doubleClicked, [this](const QModelIndex& modelIndex) {
        QDesktopServices::openUrl(QUrl(m_inputTable->cell(modelIndex.row(), 0).toString()));
    });
    connect(m_resultsTable, &Table::doubleClicked, [this](const QModelIndex& modelIndex) {
        QDesktopServices::openUrl(QUrl(m_resultsTable->cell(modelIndex.row(), 0).toString()));
    });
    connect(m_tabWidget, &QTabWidget::currentChanged, [this](int index) {
        Q_UNUSED(index)
        updateStatus();
    });
    connect(ActionsManager::instance().action(ACTION_SELECT_ALL_ROWS), &QAction::triggered, [this] { focusedTable()->selectAll(); });
    connect(ActionsManager::instance().action(ACTION_INVERT_ROWS_SELECTION), &QAction::triggered, [this] { focusedTable()->invertSelection(); });
    connect(ActionsManager::instance().action(ACTION_SELECT_GROUP), &QAction::triggered, [this] {
        bool ok;
        QString maskText = QInputDialog::getText(this, tr(TEXT_SELECT_GROUP), tr("Input Mask:"), QLineEdit::Normal, "*", &ok);
        if (ok and !maskText.isEmpty()) {
            focusedTable()->selectByMask(0, maskText);
        }
    });
    connect(ActionsManager::instance().action(ACTION_REMOVE_SELECTED_ROWS), &QAction::triggered, [this] { focusedTable()->removeSelected(); });
    connect(ActionsManager::instance().action(ACTION_REMOVE_DUPLICATE_ROWS), &QAction::triggered, [this] { focusedTable()->removeDuplicates(); });
    connect(ActionsManager::instance().action(ACTION_REMOVE_ALL_ROWS), &QAction::triggered, [this] { focusedTable()->removeAllRows(); });
    connect(ActionsManager::instance().action(ACTION_TRIM_URL_TO_ROOT), &QAction::triggered, [this] {
        focusedTable()->applyToColumn(0, [](const QString& value) -> QString {
            QUrl url(value);
            QUrl rootUrl;
            rootUrl.setScheme(url.scheme());
            rootUrl.setHost(url.host());
            return rootUrl.toString();
        });
    });
    connect(ActionsManager::instance().action(ACTION_STRIP_TRAILING_SLASH), &QAction::triggered, [this] {
        focusedTable()->applyToColumn(0, [](const QString& value) -> QString {
            return (value.endsWith("/")) ? value.mid(0, value.size() - 1) : value;
        });
    });
    connect(focusedTable(), &Table::emptied, [] {
        ActionsManager::instance().disableActions(TableActions);
    });
    connect(focusedTable(), &Table::populated, [] {
        ActionsManager::instance().enableActions(TableActions);
    });
}

Table* TablesWidget::inputTable()
{
    return m_inputTable;
}

Table* TablesWidget::resultsTable()
{
    return m_resultsTable;
}

void TablesWidget::updateResultsRow(const QVariantMap& resultData)
{
    int rowIndex = resultData["rowId"].toInt();
    QString result = resultData["result"].toString();
    m_resultsTable->appendRow(QStringList() << QString(rowIndex) << result << QString("xxx") << QString("yyy"));
}

void TablesWidget::invertSelectedRows()
{
    auto currentTable = focusedTable();
    currentTable->invertSelection();
}

void TablesWidget::removeAllRows()
{
    auto currentTable = focusedTable();
    currentTable->removeAllRows();
    // TODO: better approach to signal if the focused table is empty
    updateStatus();
}

void TablesWidget::removeDuplicatedRows()
{
    auto currentTable = focusedTable();
    currentTable->removeDuplicates();
    // TODO: better approach to signal if the focused table is empty
    updateStatus();
}

void TablesWidget::removeSelectedRows()
{
    auto currentTable = focusedTable();
    currentTable->removeSelected();
    // TODO: better approach to signal if the focused table is empty
    updateStatus();
}

void TablesWidget::updateStatus()
{
    auto* table = (m_tabWidget->currentIndex() == 0) ? m_inputTable : m_resultsTable;
    if (table) {
        if (table->rowCount() == 0)
            Q_EMIT focusedTableEmpty();
        else
            Q_EMIT focusedTableNotEmpty();
    }
}

void TablesWidget::selectAllRows()
{
    auto currentTable = focusedTable();
    currentTable->selectAll();
}

Table* TablesWidget::focusedTable()
{
    auto* table = (m_tabWidget->currentIndex() == 0) ? m_inputTable : m_resultsTable;
    //    if (table) {
    //        if (table->rowCount() == 0)
    //            Q_EMIT focusedTableEmpty();
    //        else
    //            Q_EMIT focusedTableNotEmpty();
    //    }

    return table;
}

void TablesWidget::createResultsTable(const QStringList& columns, const QList<float>& columnRatios)
{
    m_resultsTable = new Table(columns, this);
    m_resultsTable->setColumnRatios(columnRatios);
}

void TablesWidget::clearResultsTable()
{
    // TODO: Improve table creation, eg. skip creating new table if tool is not changed
}

void TablesWidget::switchToResultsTab()
{
    m_tabWidget->setCurrentIndex(1);
    updateStatus();
}

void TablesWidget::switchToSourcesTab()
{
    m_tabWidget->setCurrentIndex(0);
    updateStatus();
}
