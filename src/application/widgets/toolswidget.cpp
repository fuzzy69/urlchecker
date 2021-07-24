#include "toolswidget.h"
#include "../icons.h"
#include "../core/toolsmanager.h"

ToolsWidget::ToolsWidget ( QWidget* parent ) : QTreeWidget(parent)
{
    setMinimumWidth(200);
    setColumnCount(1);
    setHeaderLabel(tr("Tools"));
    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(current)
        Q_UNUSED(previous)
        ToolsManager::instance().setCurrentTool(current->text(0));
        Q_EMIT toolSelected(current->text(0));
    });
    // TODO:Improve tool adding logic
    for (const auto& toolName : ToolsManager::instance().tools().keys())
    {
        auto tool = ToolsManager::instance().tools().value(toolName);
        addTool(tool, tool.name() == ToolsManager::instance().currentTool().name());
    }
}

void ToolsWidget::addTool(const Tool& tool, bool current)
{
    auto item = new QTreeWidgetItem(QStringList(tool.name()));
    item->setIcon(0, tool.icon());
    insertTopLevelItem(0, item);
    if (current)
    {
        setCurrentItem(item);
    }
}
