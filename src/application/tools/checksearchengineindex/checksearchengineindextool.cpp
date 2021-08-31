#include "checksearchengineindextool.h"
#include "../../icons.h"

CheckSearchEngineIndexTool::CheckSearchEngineIndexTool()
    : Tool(
        Tools::CHECK_SEARCH_ENGINE_INDEX,
        ToolCategory::SEO,
        QIcon(QStringLiteral(ICON_BOOKMARKS)),
        QString(QStringLiteral("Check Search Engine Index")),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Index Status") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.2f)
{
}
