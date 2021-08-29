#include "toolsmanager.h"
#include "../icons.h"
#include "../tools/dummy/dummytool.h"
#include "../tools/scrapeproxies/scrapeproxiestool.h"
#include "../tools/alexarank/alexaranktool.h"
#include "../tools/test/testtool.h"
#include "../tools/scrapelinks/scrapelinkstool.h"
#include "../tools/urlstatus/urlstatustool.h"
#include "../tools/scrapesitemaps/scrapesitemapstool.h"
#include "../tools/scrapeemails/scrapeemailsstool.h"
#include "../tools/scrapephonenumbers/scrapephonenumberstool.h"
#include "../tools/checksearchengineindex/checksearchengineindextool.h"
#include "../tools/scrapeimages/scrapeimagestool.h"

ToolsManager::ToolsManager()
{
    addTool(new DummyTool);
    addTool(new TestTool);
    addTool(new ScrapeProxiesTool);
    addTool(new ScrapeLinksTool);
    addTool(new UrlStatusTool);
    addTool(new AlexaRankTool);
    addTool(new ScrapeSitemapsTool);
    addTool(new ScrapeEmailsTool);
    addTool(new ScrapePhoneNumbersTool);
    addTool(new CheckSearchEngineIndexTool);
    addTool(new ScrapeImagesTool);
    // TODO: delete these heap objects in desctructor

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
    m_toolNameToolMap.insert(tool->name(), tool);
    m_toolIdToolMap.insert(tool->id(), tool);
}

Tool& ToolsManager::currentTool() const
{
    return *m_currentTool;
}

void ToolsManager::setCurrentTool(const QString &toolName)
{
    if (m_toolNameToolMap.contains(toolName))
        m_currentTool = m_toolNameToolMap[toolName];
}

Tool& ToolsManager::getTool(Tools toolId) const
{
    return *m_toolIdToolMap[toolId];
}

Tool& ToolsManager::getTool(const QString &toolName) const
{
    return *m_toolNameToolMap[toolName];
}

QMap<QString, Tool*> ToolsManager::tools() const
{
    return m_toolNameToolMap;
}
