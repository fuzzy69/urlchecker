#pragma once

#include <QIcon>
#include <QString>

#include "tools.h"

class QMenu;

class Tool : public QObject
{
    Q_OBJECT

public:
    Tool();
    Tool(Tools toolId, ToolCategory category, const QIcon &icon, const QString &name, const QStringList &columns, const QList<float> &columnRatios);
//    virtual ~Tool();

    Tools id() const;
    ToolCategory category() const;
    QIcon icon() const;
    QString name() const;
    QStringList columns() const;
    QList<float> columnRatios() const;
    QMenu* contextMenu() const;
    void setContextMenu(QMenu* contextMenu);

protected:
    Tools m_id;
    ToolCategory m_category;
    QIcon m_icon;
    QString m_name;
    QStringList m_columns;
    QList<float> m_columnRatios;
    QMenu* m_contextMenu = nullptr;
};
