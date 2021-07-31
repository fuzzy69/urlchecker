#include "tool.h"

#include <QIcon>
#include <QMenu>
#include <QString>


Tool::Tool() :
    m_id(Tools::NONE), m_category(ToolCategory::NONE), m_icon(QIcon()), m_name(QString()), m_columns(QStringList()), m_columnRatios(QList<float>())
{
}

Tool::Tool(Tools toolId, ToolCategory category, const QIcon& icon, const QString& name, const QStringList &columns, const QList<float> &columnRatios) :
    m_id(toolId), m_category(category), m_icon(icon), m_name(name), m_columns(columns), m_columnRatios(columnRatios)
{
}

//Tool::~Tool()
//{
////    if (m_contextMenu)
////        delete m_contextMenu;
//}

Tools Tool::id() const
{
    return m_id;
}

ToolCategory Tool::category() const
{
    return m_category;
}

QIcon Tool::icon() const
{
    return m_icon;
}

QString Tool::name() const
{
    return m_name;
}

QStringList Tool::columns() const
{
    return m_columns;
}

QList<float> Tool::columnRatios() const
{
    return m_columnRatios;
}

QMenu *Tool::contextMenu() const
{
    return m_contextMenu;
}

void Tool::setContextMenu(QMenu *contextMenu)
{
    m_contextMenu = contextMenu;
}
