#include "scrapeemailsstool.h"
#include "../../icons.h"

ScrapeEmailsTool::ScrapeEmailsTool() : Tool(
    Tools::SCRAPE_EMAILS,
    ToolCategory::EXTRACTORS,
    QIcon(ICON_EMAIL),
    QString(QStringLiteral("Scrape Emails")),
    QStringList() << QStringLiteral("Email") << QStringLiteral("Source") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.3f
)
{

}
