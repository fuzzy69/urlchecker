#pragma once

#include <QIcon>
#include <QString>

#include "tools.h"


struct Tool
{
    Tool();
    Tool(Tools toolId, const QIcon &icon, const QString &name, const QStringList &columns, const QList<float> &columnRatios);

    Tools id() const;
    QIcon icon() const;
    QString name() const;
    QStringList columns() const;
    QList<float> columnRatios() const;

private:
    Tools m_id;
    QIcon m_icon;
    QString m_name;
    QStringList m_columns;
    QList<float> m_columnRatios;
};
