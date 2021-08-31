#pragma once

#include <QStateMachine>

class QState;

enum class ApplicationState {
    NONE = 0,
    APPLICATION_STARTED = 1 << 0,
    APPLICATION_IDLING = 1 << 1,
    APPLICATION_EXITING = 1 << 2,
    JOB_STARTING = 1 << 3,
    JOB_STOPPING = 1 << 4,
    JOB_FINISHING = 1 << 5,
    JOB_RUNNING = 1 << 6,
    JOB_DONE = 1 << 7,
};

class ApplicationStateMachine : public QStateMachine {
    Q_OBJECT

public:
    ApplicationStateMachine(QObject* parent = nullptr);

    ApplicationState currentState() const;

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
    QState* m_applicationStartingState;
    QState* m_applicationIdlingState;
    QState* m_applicationExitingState;
    QState* m_jobStartingState;
    QState* m_jobRunningState;
    QState* m_jobStoppingState;
    QState* m_jobFinishingState;
    QState* m_jobDoneState;
};
