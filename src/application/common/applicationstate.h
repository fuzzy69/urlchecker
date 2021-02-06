#pragma once

#include <QStateMachine>


class QState;

class ApplicationState : public QStateMachine
{
    Q_OBJECT

public:
    ApplicationState(QObject *parent = nullptr);

    void startingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
    void idleStateAssignProperty(QObject *object, const char *name, const QVariant &value);
    void exitingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
    void jobStateAssignProperty(QObject *object, const char *name, const QVariant &value);
    void jobStoppingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
    void jobFinishingStateAssignProperty(QObject *object, const char *name, const QVariant &value);

signals:
    void applicationReady();
    void applicationIdling();
    void applicationExiting();
    void jobStarting();
    void jobStopping();
    void jobFinishing();

    void applicationStarted();
    void applicationIdle();
    void applicationExit();
    void jobStarted();
    void jobRunning();
    void jobStopped();
    void jobFinished();

private:
    QState *m_startingState;
    QState *m_idleState;
    QState *m_exitingState;
    QState *m_jobStartingState;
    QState *m_jobState;
    QState *m_jobStoppingState;
    QState *m_jobFinishingState;
};
