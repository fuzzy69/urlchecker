#include "alexaranktool.h"
#include "../../icons.h"

AlexaRankTool::AlexaRankTool() : Tool(
    Tools::CHECK_ALEXA_RANK,
    QIcon(QStringLiteral(ICON_ALEXA)),
    QString(QStringLiteral("Check Alexa Rank")),
    QStringList() << QStringLiteral("URL") << QStringLiteral("Rank") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.2f
)
{

}
