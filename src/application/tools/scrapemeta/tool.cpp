#include "tool.h"
#include "../../icons.h"

ScrapeMetaTool::ScrapeMetaTool()
    : Tool(
        Tools::SCRAPE_META,
        ToolCategory::EXTRACTORS,
        QIcon(ICON_TAG_HASH),
        QString(ToolsTexts.value(Tools::SCRAPE_META)),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Title") << QStringLiteral("Description") << QStringLiteral("Keywords"),
        QList<float>() << 0.4f << 0.2f << 0.2f)
{
}
