#include "toolswidget.h"
#include "../core/toolsmanager.h"
#include "../icons.h"

//#include <QDebug>

ToolsWidget::ToolsWidget(QWidget* parent)
    : QTreeWidget(parent)
{
    setMinimumWidth(200);
    setColumnCount(1);
    setContentsMargins(0, 0, 0, 0);
    setHeaderLabel(tr("Tools"));
    for (const auto& categoryId : ToolCategoryTexts.keys())
        m_categories.insert(ToolCategoryTexts.value(categoryId));

    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem* current, QTreeWidgetItem* previous) {
        Q_UNUSED(current)
        Q_UNUSED(previous)
        if (!m_categories.contains(current->text(0))) {
            ToolsManager::instance().setCurrentTool(current->text(0));
            Q_EMIT toolSelected(current->text(0));
        }
    });
    for (const auto& categoryId : ToolCategoryTexts.keys()) {
        if (categoryId != ToolCategory::NONE) {
            QTreeWidgetItem* item = nullptr;
            item = new QTreeWidgetItem(QStringList() << ToolCategoryTexts.value(categoryId));
            insertTopLevelItem(0, item);
            m_categoriesItemMap.insert(categoryId, item);
        }
    }
    expandAll();
    // TODO:Improve tool adding logic
    for (const auto& [toolName, tool] : ToolsManager::instance().tools()) {
        addTool(tool, false);
    }
}

void ToolsWidget::addTool(const std::unique_ptr<Tool>& tool, bool current)
{
    if (m_categoriesItemMap.contains(tool.get()->category())) {
        auto item = new QTreeWidgetItem(QStringList(tool.get()->name()));
        item->setIcon(0, tool.get()->icon());
        auto* category = m_categoriesItemMap[tool.get()->category()];
        category->addChild(item);
        if (current) {
            setCurrentItem(item);
        }
    }
}

void ToolsWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        Q_EMIT toolSettingsRequested(ToolsManager::instance().currentTool());
    }
    QTreeWidget::mousePressEvent(event);
}
