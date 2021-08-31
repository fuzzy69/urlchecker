#include "scrapeimagestool.h"
#include "../../icons.h"

ScrapeImagesTool::ScrapeImagesTool()
    : Tool(
        Tools::SCRAPE_IMAGES,
        ToolCategory::SCRAPERS,
        QIcon(ICON_IMAGES),
        QString(QStringLiteral("Scrape Images")),
        QStringList() << QStringLiteral("Image Source") << QStringLiteral("Source") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.3f)
{
}
