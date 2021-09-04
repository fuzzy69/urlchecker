#include "actionsmanager.h"

ActionsManager::ActionsManager(QObject* parent)
    : QObject(parent)
    , m_actions(QMap<QString, QAction*>())
    , m_actionGroups(QMultiMap<ActionGroup, QAction*>())
{
}

ActionsManager& ActionsManager::instance()
{
    static ActionsManager instance;

    return instance;
}

void ActionsManager::addAction(const QString& actionName, QAction* action)
{
    m_actions.insert(actionName, action);
}

QAction* ActionsManager::action(const QString& actionName) const
{
    if (!m_actions.contains(actionName))
        return nullptr;

    return m_actions[actionName];
}

QAction* ActionsManager::createAction(const QString& actionName, ActionGroup actionGroup, const QString& iconFile, const QString& actionText)
{
    auto* action = new QAction(QIcon(iconFile), actionText, this);
    m_actions.insert(actionName, action);
    m_actionGroups.insert(actionGroup, action);

    return action;
}

void ActionsManager::disableActions(ActionGroup actionGroups)
{
    for (const auto& actionGroup : m_actionGroups.keys()) {
        if (static_cast<bool>(actionGroups & actionGroup)) {
            for (auto* action : m_actionGroups.values(actionGroup)) {
                action->setEnabled(false);
            }
        }
    }
}

void ActionsManager::enableActions(ActionGroup actionGroups)
{
    for (const auto& actionGroup : m_actionGroups.keys()) {
        if (static_cast<bool>(actionGroups & actionGroup)) {
            for (auto* action : m_actionGroups.values(actionGroup)) {
                action->setEnabled(true);
            }
        }
    }
}
