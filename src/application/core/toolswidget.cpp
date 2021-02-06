#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "toolswidget.h"


ToolsWidget::ToolsWidget(QWidget* parent) : QTreeWidget(parent)
{
    setFixedWidth(200);
    setColumnCount(1);
    setHeaderLabel("Tools");
    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(previous);
        emit toolSelected(QString(" %1").arg(current->text(0)));
    });
}

void ToolsWidget::addTool(const QIcon& icon, const QString& text)
{
    auto item = new QTreeWidgetItem(QStringList(text));
    item->setIcon(0, icon);
    insertTopLevelItem(0, item);
}

