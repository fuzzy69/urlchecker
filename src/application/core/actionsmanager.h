#pragma once

#include <QAction>
#include <QIcon>
#include <QMap>
#include <QMultiMap>
#include <QObject>

#include "enumbitmask.h"

class QAction;

enum class ActionGroup {
    NONE = 1 << 0,
    FILE_GROUP = 1 << 1,
    SELECTION = 1 << 2,
    EDIT = 1 << 3,
    FILTER = 1 << 4,
    WINDOW = 1 << 5,
    HELP = 1 << 6,
    SIDEBAR = 1 << 7,
    ALL = 1 << 8
};

//inline ActionGroup operator|(ActionGroup x, ActionGroup y)
//{
//    return static_cast<ActionGroup>(static_cast<int>(x) | static_cast<int>(y));
//}

class ActionsManager final : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(ActionsManager)
public:
    explicit ActionsManager(QObject* parent = nullptr);

    static ActionsManager& instance();
    void addAction(const QString& actionName, QAction* action);
    QAction* action(const QString& actionName) const;
    QAction* createAction(const QString& actionName, ActionGroup actionGroup, const QString& iconFile, const QString& actionText);
    template <typename Receiver, typename Slot>
    QAction* createAction(const QString& actionName, ActionGroup actionGroup, const QString& iconFile, const QString& actionText, const Receiver* receiver, const Slot& slot)
    {
        auto* action = new QAction(QIcon(iconFile), actionText, this);
        connect(action, &QAction::triggered, receiver, slot);
        m_actions.insert(actionName, action);
        m_actionGroups.insert(actionGroup, action);

        return action;
    }

public Q_SLOTS:
    void disableActions(ActionGroup actionGroups);
    void enableActions(ActionGroup actionGroups);

private:
    static ActionsManager* m_instance;
    QMap<QString, QAction*> m_actions;
    QMultiMap<ActionGroup, QAction*> m_actionGroups;
};
