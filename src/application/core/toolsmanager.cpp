#include "toolsmanager.h"
#include "../icons.h"
#include "../tools/dummy/dummytool.h"
#include "../tools/scrapeproxies/scrapeproxiestool.h"
#include "../tools/alexarank/alexaranktool.h"
#include "../tools/test/testtool.h"
#include "../tools/scrapelinks/scrapelinkstool.h"
#include "../tools/urlstatus/urlstatustool.h"
#include "../tools/scrapesitemaps/scrapesitemapstool.h"

ToolsManager::ToolsManager()
{
    addTool(new DummyTool);
    addTool(new TestTool);
    addTool(new ScrapeProxiesTool);
    addTool(new ScrapeLinksTool);
    addTool(new UrlStatusTool);
    addTool(new AlexaRankTool);
    addTool(new ScrapeSitemapsTool);

    setCurrentTool(QStringLiteral("Check URL Status"));
}

ToolsManager & ToolsManager::instance()
{
    static ToolsManager instance;

    return instance;
}

ToolsManager::~ToolsManager()
{

}

void ToolsManager::addTool(Tool *tool)
{
    m_tools.insert(tool->name(), tool);
}

//void ToolsManager::addTool(const Tool& tool, bool current)
//{
////    auto item = new QTreeWidgetItem(QStringList(tool.name()));
////    item->setIcon(0, tool.icon());
////    insertTopLevelItem(0, item);
//    if (current)
//    {
////        setCurrentItem(item);
//        m_currentTool = tool;
//    }
//    m_tools.insert(tool.name(), tool);
//}

Tool& ToolsManager::currentTool() const
{
    return *m_currentTool;
}

void ToolsManager::setCurrentTool(const QString &toolName)
{
    if (m_tools.contains(toolName))
        m_currentTool = m_tools[toolName];
}

Tool& ToolsManager::getTool(const QString& toolName) const
{
    return *m_tools[toolName];
}

QMap<QString, Tool*> ToolsManager::tools() const
{
    return m_tools;
}
