// #include <QTreeWidget>
// #include <QTreeWidgetItem>
// #include <QList>
// #include <QMap>
// #include <QDebug>

#include "toolswidget.h"
// #include "tool.h"
#include "../icons.h"

ToolsWidget::ToolsWidget ( QWidget* parent ) : QTreeWidget(parent)
{
//    setFixedWidth(200);
    setMinimumWidth(200);
    setColumnCount(1);
//     setHeaderLabel(tr("Tools"));
    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(current)
        Q_UNUSED(previous)
        auto tool = m_tools.value(current->text(0));
        m_currentTool = tool;
        Q_EMIT toolSelected(tool);
    });
    addTool(
        Tool(
        Tools::CHECK_URL_STATUS, 
        QIcon(ICON_CHAIN), 
        QString("Check URL Status"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5f << 0.3f
    ));
    addTool(
        Tool(
            Tools::CHECK_ALEXA_RANK, 
            QIcon(QStringLiteral(ICON_ALEXA)), 
            QString(QStringLiteral("Check Alexa Rank")),
            QStringList() << QStringLiteral("URL") << QStringLiteral("Rank") << QStringLiteral("Status"),
            QList<float>() << 0.5f << 0.3f
        )
    );
    addTool(
        Tool(
            Tools::SCRAPE_PROXIES, 
            QIcon(QStringLiteral(ICON_MASK)), 
            QString(QStringLiteral("Scrape Proxies")),
            QStringList() << QStringLiteral("Proxy") << QStringLiteral("Source") << QStringLiteral("Status"),
            QList<float>() << 0.5f << 0.3f
        )
    );
    addTool(Tool(
        Tools::TEST, 
        QIcon(ICON_HAMMER), 
        QString("Test"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5f << 0.3f
    ));
#if APP_DEBUG
    addTool(Tool(
        Tools::DUMMY, 
        QIcon(ICON_QUESTION), 
        QString("Dummy"),
        QStringList() << "URL" << "Result" << "Status",
        QList<float>() << 0.5f << 0.3f
    ));
#endif
    
}

// ToolsWidget::ToolsWidget(const Tool &defaultTool, QWidget* parent) : QTreeWidget(parent), m_tools(QMap<QString, Tool>()), m_currentTool(defaultTool)
// {
//     setFixedWidth(200);
//     setColumnCount(1);
//     setHeaderLabel("Tools");
//     connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem *current, QTreeWidgetItem *previous){
//         Q_UNUSED(current);
//         Q_UNUSED(previous);
//         auto tool = m_tools.value(current->text(0));
//         m_currentTool = tool;
//         emit toolSelected(tool);
//     });
//     addTool(defaultTool, true);
// }

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
    return m_tools[toolName];
}
