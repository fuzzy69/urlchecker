#include <QAction>
#include <QIcon>

#include "actions.h"
#include "core/actionsmanager.h"
#include "icons.h"

void createActions()
{
    ActionsManager::instance().createAction(QStringLiteral("importUrls"), QStringLiteral(ICON_TABLE_IMPORT), QStringLiteral("Import URLs"));
    ActionsManager::instance().createAction(QStringLiteral("clearRecentUrlFiles"), QStringLiteral(ICON_BROOM), QStringLiteral("Clear Recent URL Files"));
    ActionsManager::instance().createAction(QStringLiteral("exportResults"), QStringLiteral(ICON_TABLE_EXPORT), QStringLiteral("Export Results"));
    ActionsManager::instance().createAction(QStringLiteral("quit"), QStringLiteral(ICON_CONTROL_POWER), QStringLiteral("Quit"));
    ActionsManager::instance().createAction(QStringLiteral("removAllRows"), QStringLiteral(ICON_BROOM), QStringLiteral("Remove All Rows"));
    ActionsManager::instance().createAction(QStringLiteral("removeDuplicates"), QStringLiteral(ICON_TABLE_DELETE_ROW), QStringLiteral("Remove Duplicates"));
    ActionsManager::instance().createAction(QStringLiteral("selectAllRows"), QStringLiteral(ICON_TABLE_SELECT_ALL), QStringLiteral("Select All Rows"));
    ActionsManager::instance().createAction(QStringLiteral("invertRowsSelection"), QStringLiteral(ICON_TABLE), QStringLiteral("Invert Rows Selection"));
    ActionsManager::instance().createAction(QStringLiteral("removeSelectedRows"), QStringLiteral(ICON_TABLE_DELETE_ROW), QStringLiteral("Remove Selected Rows"));
    ActionsManager::instance().createAction(QStringLiteral("centerWindow"), QStringLiteral(ICON_RESIZE), QStringLiteral("Center Window"));
    ActionsManager::instance().createAction(QStringLiteral("about"), QStringLiteral(ICON_INFORMATION), QStringLiteral("About"));
//    m_importUrlsAction = new QAction(QIcon(ICON_TABLE_IMPORT), tr("Import URLs"), this);
//    m_recentUrlFilesMenu = new QMenu(tr("Open Recent URL File"), this);
//    m_clearRecentUrlFilesAction = new QAction(QIcon(ICON_BROOM), tr("Clear List"), this);
//    m_exportResultsAction = new QAction(QIcon(ICON_TABLE_EXPORT), tr("Export Results"), this);
//    m_quitAction = new QAction(QIcon(ICON_CONTROL_POWER), tr("Quit"), this);
//    m_removeAllAction = new QAction(QIcon(ICON_BROOM), tr("Remove All Rows"), this);
//    m_removeDuplicatesAction = new QAction(QIcon(ICON_TABLE_DELETE_ROW), tr("Remove Duplicates"), this);
//    m_selectAllAction = new QAction(QIcon(ICON_TABLE_SELECT_ALL), tr("Select All Rows"), this);
//    m_invertSelectionAction = new QAction(QIcon(ICON_TABLE), tr("Invert Selection"), this);
//    m_removeSelectedAction= new QAction(QIcon(ICON_TABLE_DELETE_ROW), tr("Remove Selected Rows"), this);
//    m_centerWindowAction = new QAction(QIcon(ICON_RESIZE), tr("Center Window"), this);
//    m_aboutAction = new QAction(QIcon(ICON_INFORMATION), tr("About"), this);

    // Sidebar
    ActionsManager::instance().createAction(QStringLiteral("workspace"), QStringLiteral(ICON_DESKTOP), QStringLiteral("Workspace"));
    ActionsManager::instance().createAction(QStringLiteral("settings"), QStringLiteral(ICON_GEAR), QStringLiteral("Settings"));
    ActionsManager::instance().createAction(QStringLiteral("proxies"), QStringLiteral(ICON_MASK), QStringLiteral("Proxies"));
    ActionsManager::instance().createAction(QStringLiteral("help"), QStringLiteral(ICON_QUESTION), QStringLiteral("Help"));
    //    m_workspaceAction = new QAction(QIcon(ICON_DESKTOP), tr("Workspace"), this);
    //    m_settingsAction = new QAction(QIcon(ICON_GEAR), tr("Settings"), this);
    //    m_proxiesAction = new QAction(QIcon(ICON_MASK), tr("Proxies"), this);
    //    m_helpAction = new QAction(QIcon(ICON_QUESTION), tr("Help"), this);
}
