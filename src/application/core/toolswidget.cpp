#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QMap>
#include <QDebug>

#include "toolswidget.h"
#include "tool.h"


ToolsWidget::ToolsWidget(const Tool &defaultTool, QWidget* parent) : QTreeWidget(parent), m_tools(QMap<QString, Tool>()), m_currentTool(defaultTool)
{
    setFixedWidth(200);
    setColumnCount(1);
    setHeaderLabel("Tools");
    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(current);
        Q_UNUSED(previous);
        auto tool = m_tools.value(current->text(0));
        m_currentTool = tool;
        emit toolSelected(tool);
    });
    addTool(defaultTool, true);
}

void ToolsWidget::addTool(const Tool& tool, bool current)
{
    auto item = new QTreeWidgetItem(QStringList(tool.name()));
    item->setIcon(0, tool.icon());
    insertTopLevelItem(0, item);
    if (current)
    {
        setCurrentItem(item);
        m_currentTool = tool;
    }
    m_tools.insert(tool.name(), tool);
}

Tool ToolsWidget::currentTool()
{
    return m_currentTool;
}

Tool ToolsWidget::getTool(const QString& toolName) const
{
//     qDebug() << m_tools;
    return m_tools[toolName];
}
