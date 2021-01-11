#pragma once

#include <QObject>
#include <QStringList>

class QTableView;
class QStandardItemModel;

class Table : public QObject
{
    Q_OBJECT
public:
    explicit Table(QStringList columns, QObject* parent = nullptr);
    ~Table();

    void setColumnRatios(QList<float> columnRatios);
    void resizeColumns();
    int rowCount() const;
    bool removeRow(int rowIndex);
    void removeAllRows();
    void row(int rowIndex) const;
    void appendRow(QStringList cells);
    QVariant cell(int rowIndex, int colIndex) const;
    bool setCell(int rowIndex, int colIndex, QVariant value);
    void selectAll();
    void selectNone();
    void invertSelection();
    QSet<int> selectedRows() const;

    void name(QString name);
    QString name() const;
    QTableView* tableView() const;
    QStandardItemModel* tableModel() const;
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
