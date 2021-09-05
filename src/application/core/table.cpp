#include <QAbstractItemView>
#include <QBrush>
#include <QColor>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QRegExp>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

#include <QDebug>

#include "table.h"

Table::Table(const QStringList& columns, QObject* parent)
    : QObject(parent)
    , m_columns(columns)
{
    m_tableView = new QTableView;
    m_tableModel = new QStandardItemModel;
    m_tableModel->setHorizontalHeaderLabels(m_columns);
    m_tableView->setModel(m_tableModel);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tableView->setAlternatingRowColors(true);

    connect(m_tableView, &QTableView::doubleClicked, this, &Table::doubleClicked);
    connect(m_tableView, &QTableView::customContextMenuRequested, this, &Table::onCustomContextMenuRequest);
}

Table::~Table()
{
    if (m_tableModel) {
        delete m_tableModel;
        m_tableModel = nullptr;
    }
    if (m_tableView) {
        delete m_tableView;
        m_tableView = nullptr;
    }
}

void Table::resetColumns(const QStringList& columns)
{
    m_columns = columns;
    m_tableModel->clear();
    m_tableModel->setHorizontalHeaderLabels(m_columns);
    m_tableView->setModel(m_tableModel);
}

void Table::setContextMenu(QMenu* contextMenu)
{
    m_contextMenu = contextMenu;
}

void Table::setColumn(int columnIndex, QVariant value)
{
    if (columnIndex >= 0 and columnIndex < columnCount()) {
        for (int i = 0; i < rowCount(); ++i) {
            m_tableModel->setData(m_tableModel->index(i, columnIndex), value);
        }
    }
}

void Table::applyToColumn(int columnIndex, std::function<QString(const QString&)> function)
{
    if (columnIndex >= 0 and columnIndex < columnCount()) {
        for (int i = 0; i < rowCount(); ++i) {
            QString value(m_tableModel->data(m_tableModel->index(i, columnIndex)).toString());
            QString newValue(function(value));
            m_tableModel->setData(m_tableModel->index(i, columnIndex), newValue);
        }
    }
}

void Table::setColumnRatios(const QList<float>& columnRatios)
{
    m_columnRatios = columnRatios;
}

void Table::resizeColumns()
{
    for (int i = 0; i < m_tableModel->columnCount(); ++i) {
        if (i < m_columnRatios.count())
            m_tableView->setColumnWidth(i, static_cast<int>(static_cast<float>(m_tableView->geometry().width()) * m_columnRatios.at(i)));
    }
}

int Table::rowCount() const
{
    return (m_tableModel) ? m_tableModel->rowCount() : 0;
}

int Table::columnCount() const
{
    return m_columns.count();
}

QStringList Table::columnNames() const
{
    return m_columns;
}

bool Table::removeRow(int rowIndex)
{
    return m_tableModel->removeRow(rowIndex);
}

void Table::removeAllRows()
{
    for (int i = m_tableModel->rowCount() - 1; i >= 0; --i) {
        m_tableModel->removeRow(i);
    }
    Q_EMIT emptied();
}

void Table::row(int rowIndex) const
{
    QList<QVariant> rowCells;
    for (int colIndex = 0; colIndex < m_tableModel->columnCount(); ++colIndex) {
        rowCells << m_tableModel->data(m_tableModel->index(rowIndex, colIndex));
    }
}

void Table::appendRow(QStringList cells)
{
    if (rowCount() == 0)
        Q_EMIT populated();
    QList<QStandardItem*> rowCells;
    for (QString cell : cells) {
        rowCells << new QStandardItem(cell);
    }
    m_tableModel->appendRow(rowCells);
}

QVariant Table::cell(int rowIndex, int colIndex) const
{
    return m_tableModel->data(m_tableModel->index(rowIndex, colIndex));
}

bool Table::setCell(int rowIndex, int colIndex, QVariant value)
{
    return m_tableModel->setData(m_tableModel->index(rowIndex, colIndex), value);
}

void Table::selectAll()
{
    m_tableView->selectAll();
}

void Table::selectNone()
{
    m_tableView->clearSelection();
}

void Table::invertSelection()
{
    QSet<int> selectedIndexes = selectedRows();
    if (selectedIndexes.count() == 0) {
        selectAll();
    } else if (selectedIndexes.count() == m_tableModel->rowCount()) {
        selectNone();
    } else {
        if (selectedIndexes.count() > m_tableModel->rowCount() / 2) {
            selectNone();
            for (int i = 0; i < m_tableModel->rowCount(); ++i) {
                if (!selectedIndexes.contains(i))
                    m_tableView->selectRow(i);
            }
        } else {
            selectAll();
            for (int i = 0; i < m_tableModel->rowCount(); ++i) {
                if (selectedIndexes.contains(i)) {
                    for (int j = 0; j < m_tableModel->columnCount(); ++j) {
                        m_tableView->selectionModel()->select(m_tableModel->index(i, j), QItemSelectionModel::Deselect);
                    }
                }
            }
        }
    }
}

void Table::selectByMask(int column, const QString& maskText)
{
    if (column < 0 or column >= m_columns.count())
        return;
    QRegExp regex(maskText, Qt::CaseSensitive, QRegExp::Wildcard);
    for (int i = 0; i < rowCount(); ++i) {
        QString cell = m_tableModel->data(m_tableModel->index(i, column)).toString();
        if (regex.exactMatch(cell)) {
            selectRow(i);
        }
    }
}

void Table::selectRow(int rowIndex)
{
    for (int i = 0; i < m_tableModel->columnCount(); ++i) {
        m_tableView->selectionModel()->select(m_tableModel->index(rowIndex, i), QItemSelectionModel::Select);
    }
}

QSet<int> Table::selectedRows() const
{
    QSet<int> rowsIndexes;
    for (const QModelIndex& modelIndex : m_tableView->selectionModel()->selectedIndexes()) {
        rowsIndexes.insert(modelIndex.row());
    }

    return rowsIndexes;
}

void Table::name(const QString& name)
{
    m_name = name;
}

QString Table::name() const
{
    return m_name;
}

QTableView* Table::tableView() const
{
    return m_tableView;
}

QStandardItemModel* Table::tableModel() const
{
    return m_tableModel;
}

void Table::setRowColor(int rowIndex, const QColor& textColor, const QColor& backgroundColor)
{
    for (int columnIndex = 0; columnIndex < rowCount(); ++columnIndex) {
        m_tableModel->setData(m_tableModel->index(rowIndex, columnIndex), QBrush(textColor), Qt::TextColorRole);
        m_tableModel->setData(m_tableModel->index(rowIndex, columnIndex), QBrush(backgroundColor), Qt::BackgroundColorRole);
    }
}

void Table::removeDuplicates()
{
    QSet<QString> urls;
    QSet<int> duplicateIndexes;
    QString url;
    for (int i = 0; i < rowCount(); ++i) {
        url = cell(i, 0).toString();
        if (urls.contains(url))
            duplicateIndexes.insert(i);
        else
            urls.insert(url);
    }
    for (int i = rowCount() - 1; i >= 0; --i) {
        if (duplicateIndexes.contains(i))
            removeRow(i);
    }
    if (rowCount() == 0)
        Q_EMIT emptied();
}

void Table::removeSelected()
{
    QSet<int> selectedIndexes = selectedRows();
    for (int i = rowCount() - 1; i >= 0; --i) {
        if (selectedIndexes.contains(i))
            removeRow(i);
    }
    if (rowCount() == 0)
        Q_EMIT emptied();
}

void Table::onCustomContextMenuRequest(const QPoint& pos)
{
    if (m_contextMenu) {
        m_contextMenu->popup(m_tableView->viewport()->mapToGlobal(pos));
    }
}
