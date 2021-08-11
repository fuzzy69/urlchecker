#include "scrapephonenumberstool.h"
#include "../../icons.h"

ScrapePhoneNumbersTool::ScrapePhoneNumbersTool() : Tool(
    Tools::SCRAPE_PHONE_NUMBERS,
    ToolCategory::EXTRACTORS,
    QIcon(ICON_PHONE_NUMBER),
    QString(QStringLiteral("Scrape Phone Numbers")),
    QStringList() << QStringLiteral("Phone Number") << QStringLiteral("Source") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.3f
)
{

}
