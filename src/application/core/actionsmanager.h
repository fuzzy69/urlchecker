#pragma once

#include <QAction>
#include <QIcon>
#include <QMap>
#include <QObject>

class QAction;

class ActionsManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionsManager)
public:
    explicit ActionsManager(QObject *parent = nullptr);
//    ~ActionsManager() override;

    static ActionsManager& instance();
    void addAction(const QString& actionName, QAction* action);
    QAction* action(const QString& actionName) const;
//    QAction* createAction(const QString& actionName, const QString& iconFile, const QString& actionText, const QObject* receiver, const QMetaMethod& method);
    QAction* createAction(const QString& actionName, const QString& iconFile, const QString& actionText);
    template<typename Receiver, typename Slot>
    QAction* createAction(const QString& actionName, const QString& iconFile, const QString& actionText, const Receiver* receiver, const Slot& slot)
    {
        auto* action = new QAction(QIcon(iconFile), actionText, this);
        connect(action, &QAction::triggered, receiver, slot);
        m_actions.insert(actionName, action);

        return action;
    }

//Q_SIGNALS:
//    void currentActionIndexChanged(int index);

private:
    static ActionsManager* m_instance;
    QMap<QString, QAction*> m_actions;
};
