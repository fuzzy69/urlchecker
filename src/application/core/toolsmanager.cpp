#include "toolsmanager.h"
#include "../icons.h"

ToolsManager::ToolsManager()
{
    addTool(
        Tool(
        Tools::CHECK_URL_STATUS,
        QIcon(ICON_CHAIN),
        QString(QStringLiteral("Check URL Status")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f
    ));
    addTool(
        Tool(
            Tools::CHECK_ALEXA_RANK,
            QIcon(QStringLiteral(ICON_ALEXA)),
            QString(QStringLiteral("Check Alexa Rank")),
            QStringList() << QStringLiteral("URL") << QStringLiteral("Rank") << QStringLiteral("Details"),
            QList<float>() << 0.5f << 0.2f
        )
    );
    addTool(
        Tool(
            Tools::SCRAPE_PROXIES,
            QIcon(QStringLiteral(ICON_MASK)),
            QString(QStringLiteral("Scrape Proxies")),
            QStringList() << QStringLiteral("Proxy") << QStringLiteral("Source") << QStringLiteral("Details"),
            QList<float>() << 0.5f << 0.3f
        )
    );
    addTool(Tool(
        Tools::TEST,
        QIcon(ICON_HAMMER),
        QString(QStringLiteral("Test")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f
    ));
    addTool(Tool(
        Tools::SCRAPE_LINKS,
        QIcon(ICON_CHAIN),
        QString(QStringLiteral("Scrape Links")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f
    ));
#if APP_DEBUG
    addTool(Tool(
        Tools::DUMMY,
        QIcon(ICON_QUESTION),
        QString(QStringLiteral("Dummy")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Source") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f
    ));
#endif
    setCurrentTool(QStringLiteral("Check URL Status"));
}

ToolsManager & ToolsManager::instance()
{
    static ToolsManager instance;

    return instance;
}

void ToolsManager::addTool(const Tool& tool, bool current)
{
//    auto item = new QTreeWidgetItem(QStringList(tool.name()));
//    item->setIcon(0, tool.icon());
//    insertTopLevelItem(0, item);
    if (current)
    {
//        setCurrentItem(item);
        m_currentTool = tool;
    }
    m_tools.insert(tool.name(), tool);
}

Tool ToolsManager::currentTool() const
{
    return m_currentTool;
}

void ToolsManager::setCurrentTool(const QString &toolName)
{
    if (m_tools.contains(toolName))
        m_currentTool = m_tools[toolName];
}

Tool ToolsManager::getTool(const QString& toolName) const
{
    return m_tools[toolName];
}

QMap<QString, Tool> ToolsManager::tools() const
{
    return m_tools;
}
