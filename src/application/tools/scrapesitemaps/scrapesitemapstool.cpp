#include "scrapesitemapstool.h"
#include "../../icons.h"

ScrapeSitemapsTool::ScrapeSitemapsTool()
    : Tool(
        Tools::SCRAPE_SITEMAPS,
        ToolCategory::URL,
        QIcon(ICON_SITEMAP),
        QString(QStringLiteral("Scrape Sitemaps")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Website") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.3f)
{
}
