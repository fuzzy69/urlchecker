#pragma once

#include <QStateMachine>


class QState;

class ApplicationStateMachine : public QStateMachine
{
    Q_OBJECT

public:
    ApplicationStateMachine(QObject *parent = nullptr);

    QString currentState() const;
//     void startingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
//     void idleStateAssignProperty(QObject *object, const char *name, const QVariant &value);
//     void exitingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
//     void jobStateAssignProperty(QObject *object, const char *name, const QVariant &value);
//     void jobStoppingStateAssignProperty(QObject *object, const char *name, const QVariant &value);
//     void jobFinishingStateAssignProperty(QObject *object, const char *name, const QVariant &value);

Q_SIGNALS:
    void applicationStarted();
    void applicationIdling();
    void applicationExiting();
    void jobStarted();
    void jobRunning();
    void jobStopping();
    void jobFinished();

    void applicationStart();
    void applicationReady();
    void applicationExit();
    void jobStart();
    void jobRun();
    void jobStop();
    void jobFinish();
    void jobDone();

private:
    QState *m_applicationStartingState;
    QState *m_applicationIdlingState;
    QState *m_applicationExitingState;
    QState *m_jobStartingState;
    QState *m_jobRunningState;
    QState *m_jobStoppingState;
    QState *m_jobFinishingState;
    QState *m_jobDoneState;
};
