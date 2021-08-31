#include "urlstatustool.h"
#include "../../icons.h"

UrlStatusTool::UrlStatusTool()
    : Tool(
        Tools::CHECK_URL_STATUS,
        ToolCategory::URL,
        QIcon(ICON_CHAIN),
        QString(QStringLiteral("Check URL Status")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f)
{
}
