#pragma once

#include <memory>

#include <QMap>
#include <QMouseEvent>
#include <QSet>
#include <QTreeWidget>

#include "../tools/tool.h"
#include "../tools/tools.h"

class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(QWidget *parent = nullptr);

    void addTool(const std::unique_ptr<Tool>& tool, bool current = false);

signals:
    void toolSelected(const QString &toolText);
    void toolSelected(const Tool &tool);
    void toolSettingsRequested(const Tool &tool);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMap<QString, Tool> m_tools;
    Tool m_currentTool;
    QSet<QString> m_categories;
    QMap<ToolCategory, QTreeWidgetItem*> m_categoriesItemMap;
};
