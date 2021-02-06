#pragma once

#include <QTreeWidget>

class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(QWidget *parent = nullptr);

    void addTool(const QIcon &icon, const QString &text);

signals:
    void toolSelected(const QString &toolText);
};
