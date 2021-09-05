#include <QAction>
#include <QMenu>
#include <QMetaMethod>

#include <QDebug>

#include "../../core/actionsmanager.h"
#include "../../core/applicationbridge.h"
#include "../../core/table.h"
#include "../../icons.h"
#include "../../widgets/proxieswidget.h"
#include "../../widgets/tableswidget.h"
#include "scrapeproxiestool.h"

ScrapeProxiesTool::ScrapeProxiesTool()
    : Tool(
        Tools::SCRAPE_PROXIES,
        ToolCategory::EXTRACTORS,
        QIcon(QStringLiteral(ICON_MASK)),
        QString(QStringLiteral("Scrape Proxies")),
        QStringList() << QStringLiteral("Proxy") << QStringLiteral("Source") << QStringLiteral("Details"),
        QList<float>() << 0.5f << 0.3f)
{
    m_contextMenu = new QMenu;
    QAction* action = nullptr;
    action = ActionsManager::instance().createAction(QStringLiteral("ADD_PROXIES_TO_PROXIES_LIST"), ActionGroup::NONE, QStringLiteral(ICON_MASK), QStringLiteral("Add to application proxies list"), this, &ScrapeProxiesTool::addProxiesToProxiesList);
    m_contextMenu->addAction(action);
    action = ActionsManager::instance().createAction(QStringLiteral("COPY_PROXIES_TO_SOURCE_TABLE"), ActionGroup::NONE, QStringLiteral(ICON_DOCUMENT_LIST), QStringLiteral("Copy proxies to source URLs table"), this, &ScrapeProxiesTool::copyProxiesToSourcesTable);
    m_contextMenu->addAction(action);
}

void ScrapeProxiesTool::addProxiesToProxiesList()
{
    // TODO: Improve efficiency
    auto* resultsTable = ApplicationBridge::instance().tablesWidget()->resultsTable();
    auto* proxiesWidget = ApplicationBridge::instance().proxiesWidget();
    for (int i = 0; i < resultsTable->rowCount(); ++i) {
        proxiesWidget->append(resultsTable->cell(i, 0).toString());
    }
}

void ScrapeProxiesTool::copyProxiesToSourcesTable()
{
    // TODO: Improve efficiency
    auto* resultsTable = ApplicationBridge::instance().tablesWidget()->resultsTable();
    auto* inputTable = ApplicationBridge::instance().tablesWidget()->inputTable();
    for (int i = 0; i < resultsTable->rowCount(); ++i) {
        inputTable->appendRow(QStringList() << resultsTable->cell(i, 0).toString() << QString(""));
    }
}
