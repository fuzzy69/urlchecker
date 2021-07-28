#include <QAction>
#include <QMenu>

#include "dummytool.h"
#include "../../icons.h"

DummyTool::DummyTool() : Tool(
    Tools::DUMMY,
    QIcon(ICON_QUESTION),
    QStringLiteral("Dummy"),
    QStringList() << QStringLiteral("URL") << QStringLiteral("Source") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.1f
)
{
    m_contextMenu = new QMenu;
    m_contextMenu->addAction(new QAction("Dummy action 1", nullptr));
    m_contextMenu->addAction(new QAction("Dummy action 2", nullptr));
}
