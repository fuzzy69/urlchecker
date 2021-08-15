#include "testtool.h"
#include "../../icons.h"

TestTool::TestTool() : Tool(
    Tools::TEST,
    ToolCategory::MISC,
    QIcon(ICON_HAMMER),
    QString(QStringLiteral("Test")),
    QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.3f
)
{

}
