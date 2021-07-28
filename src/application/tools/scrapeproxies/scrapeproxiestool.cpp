#include <QAction>
#include <QMenu>
#include <QMetaMethod>

#include <QDebug>

#include "scrapeproxiestool.h"
#include "../../icons.h"
#include "../../core/actionsmanager.h"
#include "../../core/applicationbridge.h"
#include "../../core/table.h"
#include "../../widgets/proxieswidget.h"
#include "../../widgets/tableswidget.h"

ScrapeProxiesTool::ScrapeProxiesTool() : Tool(
    Tools::SCRAPE_PROXIES,
    QIcon(QStringLiteral(ICON_MASK)),
    QString(QStringLiteral("Scrape Proxies")),
    QStringList() << QStringLiteral("Proxy") << QStringLiteral("Source") << QStringLiteral("Details"),
    QList<float>() << 0.5f << 0.3f
)
{
    m_contextMenu = new QMenu;
    QAction* action = nullptr;
//    action = new QAction(QIcon(ICON_MASK), "Add to application proxies list 2", &ActionsManager::instance());
//    ActionsManager::instance().addAction(QStringLiteral("ADD_PROXIES_TO_PROXIES_LIST"), action);
    action = ActionsManager::instance().createAction(QStringLiteral("ADD_PROXIES_TO_PROXIES_LIST"), QStringLiteral(ICON_MASK), QStringLiteral("Add to application proxies list"), this, &ScrapeProxiesTool::addProxiesToProxiesList);
    m_contextMenu->addAction(action);
////    action = new QAction(QIcon(ICON_DOCUMENT_LIST), "Copy proxies to source URLs table 2", &ActionsManager::instance());
////    ActionsManager::instance().addAction(QStringLiteral("COPY_PROXIES_TO_SOURCE_TABLE"), action);
    action = ActionsManager::instance().createAction(QStringLiteral("COPY_PROXIES_TO_SOURCE_TABLE"), QStringLiteral(ICON_DOCUMENT_LIST), QStringLiteral("Copy proxies to source URLs table"), this, &ScrapeProxiesTool::copyProxiesToSourcesTable);
    m_contextMenu->addAction(action);

//    connect
//    m_contextMenu->addAction(new QAction(QIcon(ICON_MASK), "Add to application proxies list", nullptr));
    //    m_contextMenu->addAction(new QAction(QIcon(ICON_DOCUMENT_LIST), "Copy proxies to source URLs table", nullptr));
}

void ScrapeProxiesTool::addProxiesToProxiesList()
{
    qDebug() << "Add proxies ...";
    // TODO: Improve efficiency
    auto* resultsTable = ApplicationBridge::instance().tablesWidget()->resultsTable();
    auto* proxiesWidget = ApplicationBridge::instance().proxiesWidget();
    for (int i = 0; i < resultsTable->rowCount(); ++i)
    {
        proxiesWidget->append(resultsTable->cell(i, 0).toString());
    }
}

void ScrapeProxiesTool::copyProxiesToSourcesTable()
{
    qDebug() << "Copy proxies ...";
    // TODO: Improve efficiency
    auto* resultsTable = ApplicationBridge::instance().tablesWidget()->resultsTable();
    auto* inputTable = ApplicationBridge::instance().tablesWidget()->inputTable();
    for (int i = 0; i < resultsTable->rowCount(); ++i)
    {
        inputTable->appendRow(QStringList() << resultsTable->cell(i, 0).toString() << QString(""));
    }
}

