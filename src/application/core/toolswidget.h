#pragma once

#include <QTreeWidget>

#include "tool.h"
// #include "tools.h"


class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(const Tool &defaultTool, QWidget *parent = nullptr);

//     void addTool(const QIcon &icon, const QString &text);
    void addTool(const Tool &tool);
    Tool currentTool();

signals:
//     void toolSelected(const QString &toolText);
    void toolSelected(const Tool &tool);

private:
//     QList<Tool> m_tools;
    QMap<QString, Tool> m_tools;
    Tool m_currentTool;
};
