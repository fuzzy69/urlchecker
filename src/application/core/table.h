﻿#pragma once

#include <QColor>
#include <QObject>
#include <QStringList>

class QTableView;
class QStandardItemModel;

class Table : public QObject
{
    Q_OBJECT
public:
    explicit Table(const QStringList& columns, QObject* parent = nullptr);
    ~Table() override;

    void setColumnRatios(const QList<float>& columnRatios);
    void resizeColumns();
    int rowCount() const;
    int columnCount() const;
    QStringList columnNames() const;
    bool removeRow(int rowIndex);
    void removeAllRows();
    void removeDuplicates();
    void removeSelected();
    void row(int rowIndex) const;
    void appendRow(QStringList cells);
    QVariant cell(int rowIndex, int colIndex) const;
    bool setCell(int rowIndex, int colIndex, QVariant value);
    void selectAll();
    void selectNone();
    void invertSelection();
    QSet<int> selectedRows() const;
    void setRowColor(int rowIndex, const QColor &textColor, const QColor &backgroundColor);
    void name(const QString& name);
    QString name() const;
    QTableView* tableView() const;
    QStandardItemModel* tableModel() const;
    void resetColumns(const QStringList &columns);

signals:
    void doubleClicked(const QModelIndex &modelIndex);

public slots:

private:
    QTableView* m_tableView;
    QStandardItemModel* m_tableModel;
    QStringList m_columns;
    QList<float> m_columnRatios;
    QString m_name;
};
