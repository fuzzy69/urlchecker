#include "tool.h"
#include "../../icons.h"

ScrapeHtmlTool::ScrapeHtmlTool()
    : Tool(
        Tools::SCRAPE_HTML,
        ToolCategory::SCRAPERS,
        QIcon(ICON_DOCUMENT_NUMBER),
        QString(QStringLiteral("Scrape HTML")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.2f)
{
}
