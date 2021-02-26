#pragma once

#include <QIcon>
#include <QString>

#include "tools.h"


struct Tool
{
    Tool();
    Tool(Tools toolId, const QIcon &icon, const QString &name);

    Tools id() const;
    QIcon icon() const;
    QString name() const;

private:
    Tools m_id;
    QIcon m_icon;
    QString m_name;
};
