#pragma once

#include <QTreeWidget>

#include "../tools/tool.h"
#include "../tools/tools.h"

class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(QWidget *parent = nullptr);
//     explicit ToolsWidget(const Tool &defaultTool, QWidget *parent = nullptr);

    void addTool(const QIcon &icon, const QString &text);
    void addTool(const Tool &tool, bool current = false);
    Tool currentTool();
    Tool getTool(const QString &toolName) const;

signals:
    void toolSelected(const QString &toolText);
    void toolSelected(const Tool &tool);

private:
    QMap<QString, Tool> m_tools;
    Tool m_currentTool;
};
