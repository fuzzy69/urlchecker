#pragma once

#include <QTreeWidget>

#include "../tools/tool.h"

class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(QWidget *parent = nullptr);

    void addTool(const Tool &tool, bool current = false);

signals:
    void toolSelected(const QString &toolText);
    void toolSelected(const Tool &tool);

private:
    QMap<QString, Tool> m_tools;
    Tool m_currentTool;
};
