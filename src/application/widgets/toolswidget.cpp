#include "toolswidget.h"
#include "../icons.h"
#include "../core/toolsmanager.h"

#include <QDebug>

ToolsWidget::ToolsWidget ( QWidget* parent ) : QTreeWidget(parent)
{
    setMinimumWidth(200);
    setColumnCount(1);
    setHeaderLabel(tr("Tools"));
    m_categories.insert(QStringLiteral("Misc"));
    m_categories.insert(QStringLiteral("SEO"));
    m_categories.insert(QStringLiteral("Extractors"));
    m_categories.insert(QStringLiteral("Scrapers"));
    m_categories.insert(QStringLiteral("URL"));

//    m_categoriesItemMap.insert({ToolCategory::EXTRACTORS})
    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(current)
        Q_UNUSED(previous)
        if (!m_categories.contains(current->text(0)))
        {
//            qDebug() << current->text(0);
            ToolsManager::instance().setCurrentTool(current->text(0));
            Q_EMIT toolSelected(current->text(0));
        }
    });
     QTreeWidgetItem* item = nullptr;
    item = new QTreeWidgetItem(QStringList("Extractors"));
    insertTopLevelItem(0, item);
    m_categoriesItemMap.insert(ToolCategory::EXTRACTORS, item);
    item = new QTreeWidgetItem(QStringList("URL"));
    insertTopLevelItem(0, item);
    m_categoriesItemMap.insert(ToolCategory::URL, item);
    item = new QTreeWidgetItem(QStringList("SEO"));
    insertTopLevelItem(0, item);
    m_categoriesItemMap.insert(ToolCategory::SEO, item);
    item = new QTreeWidgetItem(QStringList("Misc"));
    insertTopLevelItem(0, item);
    m_categoriesItemMap.insert(ToolCategory::MISC, item);
    item = new QTreeWidgetItem(QStringList("Scrapers"));
    insertTopLevelItem(0, item);
    m_categoriesItemMap.insert(ToolCategory::SCRAPERS, item);
    expandAll();
    // TODO:Improve tool adding logic
    for (const auto& toolName : ToolsManager::instance().tools().keys())
    {
        Tool* tool = ToolsManager::instance().tools().value(toolName);
        addTool(*tool, false);
//        addTool(*tool, tool->name() == ToolsManager::instance().currentTool().name());
    }
}

//void ToolsWidget::addTool(const Tool& tool, bool current)
void ToolsWidget::addTool(Tool& tool, bool current)
{
    if (m_categoriesItemMap.contains(tool.category()))
    {
        auto item = new QTreeWidgetItem(QStringList(tool.name()));
        item->setIcon(0, tool.icon());
        auto* category = m_categoriesItemMap[tool.category()];
        category->addChild(item);
        if (current)
        {
            setCurrentItem(item);
        }
    }
//    auto item = new QTreeWidgetItem(QStringList(tool.name()));
//    item->setIcon(0, tool.icon());
//    insertTopLevelItem(0, item);
//    if (current)
//    {
//        setCurrentItem(item);
//    }
}
