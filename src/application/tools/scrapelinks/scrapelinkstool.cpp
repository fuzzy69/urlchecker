#include "scrapelinkstool.h"
#include "../../icons.h"

ScrapeLinksTool::ScrapeLinksTool()
    : Tool(
        Tools::SCRAPE_LINKS,
        ToolCategory::SCRAPERS,
        QIcon(ICON_CHAIN),
        QString(QStringLiteral("Scrape Links")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Source") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f)
{
}
