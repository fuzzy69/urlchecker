#include "actionsmanager.h"

ActionsManager::ActionsManager(QObject* parent)
    : QObject(parent)
    , m_actions(QMap<QString, QAction*>())
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

QAction* ActionsManager::createAction(const QString& actionName, const QString& iconFile, const QString& actionText)
{
    auto* action = new QAction(QIcon(iconFile), actionText, this);
    m_actions.insert(actionName, action);

    return action;
}
