#include "toolsmanager.h"
#include "../icons.h"
#include "../tools/alexarank/alexaranktool.h"
#include "../tools/checksearchengineindex/checksearchengineindextool.h"
#include "../tools/dummy/dummytool.h"
#include "../tools/scrapeemails/scrapeemailsstool.h"
#include "../tools/scrapehtml/tool.h"
#include "../tools/scrapeimages/scrapeimagestool.h"
#include "../tools/scrapelinks/scrapelinkstool.h"
#include "../tools/scrapemeta/tool.h"
#include "../tools/scrapephonenumbers/scrapephonenumberstool.h"
#include "../tools/scrapeproxies/scrapeproxiestool.h"
#include "../tools/scrapesitemaps/scrapesitemapstool.h"
#include "../tools/test/testtool.h"
#include "../tools/urlstatus/urlstatustool.h"

ToolsManager::ToolsManager()
    : m_toolIdToolMap(std::unordered_map<Tools, std::unique_ptr<Tool>>())
    , m_currentTool(Tools::NONE)
{
    addTool(std::unique_ptr<Tool>(new DummyTool));
    addTool(std::unique_ptr<Tool>(new TestTool));
    addTool(std::unique_ptr<Tool>(new ScrapeProxiesTool));
    addTool(std::unique_ptr<Tool>(new ScrapeLinksTool));
    addTool(std::unique_ptr<Tool>(new UrlStatusTool));
    addTool(std::unique_ptr<Tool>(new AlexaRankTool));
    addTool(std::unique_ptr<Tool>(new ScrapeSitemapsTool));
    addTool(std::unique_ptr<Tool>(new ScrapeEmailsTool));
    addTool(std::unique_ptr<Tool>(new ScrapePhoneNumbersTool));
    addTool(std::unique_ptr<Tool>(new CheckSearchEngineIndexTool));
    addTool(std::unique_ptr<Tool>(new ScrapeImagesTool));
    addTool(std::unique_ptr<Tool>(new ScrapeHtmlTool));
    addTool(std::unique_ptr<Tool>(new ScrapeMetaTool));

    setCurrentTool(ToolsTexts.value(Tools::CHECK_URL_STATUS));
}

ToolsManager& ToolsManager::instance()
{
    static ToolsManager instance;

    return instance;
}

void ToolsManager::setCurrentTool(const QString& toolName)
{
    for (const auto& [toolId, tool] : m_toolIdToolMap) {
        if (tool.get()->name() == toolName)
            m_currentTool = toolId;
    }
}

void ToolsManager::addTool(std::unique_ptr<Tool> tool)
{
    m_toolIdToolMap.insert({ tool.get()->id(), std::move(tool) });
}

Tool& ToolsManager::currentTool()
{
    return *m_toolIdToolMap[m_currentTool].get();
}

Tool* ToolsManager::getTool(Tools toolId)
{
    if (m_toolIdToolMap.count(toolId) == 0)
        return nullptr;

    return m_toolIdToolMap[toolId].get();
}

Tool* ToolsManager::getTool(const QString& toolName)
{
    for (const auto& [toolId, tool] : m_toolIdToolMap) {
        if (tool.get()->name() == toolName)
            return tool.get();
    }

    return nullptr;
}

std::unordered_map<Tools, std::unique_ptr<Tool>>& ToolsManager::tools()
{
    return m_toolIdToolMap;
}
