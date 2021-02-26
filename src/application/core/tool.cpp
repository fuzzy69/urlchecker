#include "tool.h"

#include <QIcon>
#include <QString>


Tool::Tool() :
    m_id(Tools::NONE), m_icon(QIcon()), m_name(QString())
{
}

Tool::Tool(Tools toolId, const QIcon& icon, const QString& name) :
    m_id(toolId), m_icon(icon), m_name(name)
{
}

Tools Tool::id() const
{
    return m_id;
}

QIcon Tool::icon() const
{
    return m_icon;
}

QString Tool::name() const
{
    return m_name;
}
