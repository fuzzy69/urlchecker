#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QDebug>
#include <QHeaderView>

#include "table.h"

Table::Table(QStringList columns, QObject *parent) : m_columns(columns), QObject(parent)
{
    m_tableView = new QTableView;
    m_tableModel = new QStandardItemModel;
    m_tableModel->setHorizontalHeaderLabels(m_columns);
    m_tableView->setModel(m_tableModel);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Table::~Table()
{
    if (m_tableModel)
        delete m_tableModel;
    if (m_tableView)
        delete m_tableView;
}

void Table::setColumnRatios(QList<float> columnRatios)
{
    m_columnRatios = columnRatios;
}

void Table::resizeColumns()
{
    for (int i = 0; i < m_tableModel->columnCount(); ++i)
    {
        if (i < m_columnRatios.count())
            m_tableView->setColumnWidth(i, m_tableView->geometry().width() * m_columnRatios.at(i));
    }
}

int Table::rowCount() const
{
    return m_tableModel->rowCount();
}

bool Table::removeRow(int rowIndex)
{
    return m_tableModel->removeRow(rowIndex);
}

void Table::removeAllRows()
{
    m_tableModel->clear();
}

void Table::row(int rowIndex) const
{
    QList<QVariant> rowCells;
    for (int colIndex = 0; colIndex < m_tableModel->columnCount(); ++colIndex)
    {
        rowCells << m_tableModel->data(m_tableModel->index(rowIndex, colIndex));
    }
    qDebug() << rowCells.count();
}

void Table::appendRow(QStringList cells)
{
//    QStandardItem
    QList<QStandardItem*> rowCells;
    for (QString cell : cells)
    {
        rowCells << new QStandardItem(cell);
        qDebug() << cell;
    }
    qDebug() << rowCells.count();
    m_tableModel->appendRow(rowCells);
}

bool Table::cell(int rowIndex, int colIndex, QVariant value)
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
    selectNone();
    for (int rowIndex = 0; rowIndex < m_tableModel->rowCount(); ++rowIndex) {
//        if (!selectedIndexes.contains(rowIndex))
//        {
            m_tableView->selectRow(rowIndex);
            qDebug() << "Select row " << rowIndex;
//        }
    }
}

QSet<int> Table::selectedRows() const
{
    QSet<int> rowsIndexes;
    for (const QModelIndex& modelIndex : m_tableView->selectionModel()->selectedIndexes())
    {
        rowsIndexes.insert(modelIndex.row());
    }

    return rowsIndexes;
}

void Table::name(QString name)
{
    m_name = name;
}

QString Table::name() const
{
    return m_name;
}

QTableView *Table::tableView() const
{
    return m_tableView;
}
