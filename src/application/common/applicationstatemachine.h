#pragma once

#include <QStateMachine>


class QState;

class ApplicationStateMachine : public QStateMachine
{
    Q_OBJECT

public:
    ApplicationStateMachine(QObject *parent = nullptr);

    QString currentState() const;

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
