#include <QAction>
#include <QIcon>

#include "actions.h"
#include "core/actionsmanager.h"
#include "icons.h"
#include "texts.h"

void createActions()
{
    ActionsManager::instance().createAction(QStringLiteral(ACTION_IMPORT_URLS), QStringLiteral(ICON_TABLE_IMPORT), QStringLiteral("Import URLs"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_CLEAR_RECENT_URL_FILES), QStringLiteral(ICON_BROOM), QStringLiteral("Clear Recent URL Files"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_EXPORT_RESULTS), QStringLiteral(ICON_TABLE_EXPORT), QStringLiteral("Export Results"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_QUIT), QStringLiteral(ICON_CONTROL_POWER), QStringLiteral("Quit"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_REMOVE_ALL_ROWS), QStringLiteral(ICON_BROOM), QStringLiteral("Remove All Rows"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_REMOVE_DUPLICATE_ROWS), QStringLiteral(ICON_TABLE_DELETE_ROW), QStringLiteral("Remove Duplicates"));

    ActionsManager::instance().createAction(QStringLiteral(ACTION_SELECT_ALL_ROWS), QStringLiteral(ICON_TABLE_SELECT_ALL), QStringLiteral("Select All Rows"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_INVERT_ROWS_SELECTION), QStringLiteral(ICON_TABLE), QStringLiteral("Invert Rows Selection"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_SELECT_GROUP), QStringLiteral(ICON_TABLE_SELECT_GROUP), QStringLiteral(TEXT_SELECT_GROUP));

    ActionsManager::instance().createAction(QStringLiteral(ACTION_REMOVE_SELECTED_ROWS), QStringLiteral(ICON_TABLE_DELETE_ROW), QStringLiteral("Remove Selected Rows"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_CENTER_WINDOW), QStringLiteral(ICON_RESIZE), QStringLiteral("Center Window"));
    ActionsManager::instance().createAction(QStringLiteral(ACTION_ABOUT), QStringLiteral(ICON_INFORMATION), QStringLiteral("About"));

    // Sidebar
    ActionsManager::instance().createAction(QStringLiteral("workspace"), QStringLiteral(ICON_DESKTOP), QStringLiteral(TEXT_WORKSPACE));
    ActionsManager::instance().createAction(QStringLiteral("settings"), QStringLiteral(ICON_GEAR), QStringLiteral(TEXT_SETTINGS));
    ActionsManager::instance().createAction(QStringLiteral("userAgents"), QStringLiteral(ICON_USER_SILHOUETTE_QUESTION), QStringLiteral(TEXT_USER_AGENTS));
    ActionsManager::instance().createAction(QStringLiteral("proxies"), QStringLiteral(ICON_MASK), QStringLiteral(TEXT_PROXIES));
    ActionsManager::instance().createAction(QStringLiteral("help"), QStringLiteral(ICON_QUESTION), QStringLiteral(TEXT_HELP));
}
