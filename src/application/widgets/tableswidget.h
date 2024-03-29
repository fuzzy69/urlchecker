﻿#pragma once

#include <QWidget>

class QTabWidget;
class QVBoxLayout;

class Table;

class TablesWidget final : public QWidget {
    Q_OBJECT
public:
    explicit TablesWidget(QWidget* parent = nullptr);

    Table* inputTable();
    Table* resultsTable();
    Table* focusedTable();

Q_SIGNALS:
    void focusedTableEmpty();
    void focusedTableNotEmpty();

public Q_SLOTS:
    void updateResultsRow(const QVariantMap& resultData);
    void createResultsTable(const QStringList& columns, const QList<float>& columnRatios);
    void clearResultsTable();
    void switchToResultsTab();
    void switchToSourcesTab();

    void removeAllRows();
    void selectAllRows();
    void invertSelectedRows();
    void removeDuplicatedRows();
    void removeSelectedRows();

private:
    // TODO: choose better name for function
    void updateStatus();

    QVBoxLayout* m_mainLayout;

    Table* m_inputTable;
    Table* m_resultsTable;
    QTabWidget* m_tabWidget;
};
