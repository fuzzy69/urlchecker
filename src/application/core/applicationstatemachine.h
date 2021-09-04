#pragma once

#include <QStateMachine>

#include "enumbitmask.h"

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

static const ApplicationState JobActiveStates = ApplicationState::JOB_STARTING | ApplicationState::JOB_RUNNING | ApplicationState::JOB_STOPPING | ApplicationState::JOB_FINISHING;

static const QMap<ApplicationState, QString> ApplicationStates {
    { ApplicationState::NONE, QStringLiteral("None") },
    { ApplicationState::APPLICATION_STARTED, QStringLiteral("Application started") },
    { ApplicationState::APPLICATION_IDLING, QStringLiteral("Ready") },
    { ApplicationState::APPLICATION_EXITING, QStringLiteral("Exiting application ...") },
    { ApplicationState::JOB_STARTING, QStringLiteral("Job starting ...") },
    { ApplicationState::JOB_STOPPING, QStringLiteral("Job stopping ...") },
    { ApplicationState::JOB_FINISHING, QStringLiteral("Job finishing ...") },
    { ApplicationState::JOB_RUNNING, QStringLiteral("Job running ...") },
    { ApplicationState::JOB_DONE, QStringLiteral("Job done") }
};

class ApplicationStateMachine final : public QStateMachine {
    Q_OBJECT

public:
    ApplicationStateMachine(QObject* parent = nullptr);

    ApplicationState currentState() const;
    QString currentStateText() const;

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
