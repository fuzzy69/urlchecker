﻿#include <QAction>
#include <QMenu>

#include "../../icons.h"
#include "dummytool.h"

DummyTool::DummyTool()
    : Tool(
        Tools::DUMMY,
        ToolCategory::MISC,
        QIcon(ICON_QUESTION),
        QStringLiteral("Dummy"),
        QStringList() << QStringLiteral("URL") << QStringLiteral("Result") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.1f)
{
    //    m_contextMenu = new QMenu;
    //    m_contextMenu->addAction(new QAction("Dummy action 1", nullptr));
    //    m_contextMenu->addAction(new QAction("Dummy action 2", nullptr));
}
