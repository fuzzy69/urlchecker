#include <QStateMachine>
#include <QState>

#include "applicationstatemachine.h"


ApplicationStateMachine::ApplicationStateMachine(QObject *parent) : QStateMachine(parent)
{
    // States
    m_applicationStartingState = new QState;
    m_applicationIdlingState = new QState;
    m_applicationExitingState = new QState;
    m_jobStartingState = new QState;
    m_jobRunningState = new QState;
    m_jobStoppingState = new QState;
    m_jobFinishingState = new QState;
    m_jobDoneState = new QState;

    addState(m_applicationStartingState);
    addState(m_applicationIdlingState);
    addState(m_applicationExitingState);
    addState(m_jobStartingState);
    addState(m_jobRunningState);
    addState(m_jobStoppingState);
    addState(m_jobFinishingState);
    addState(m_jobDoneState);

    // Transitions
    m_applicationStartingState->addTransition(this, &ApplicationStateMachine::applicationReady, m_applicationIdlingState);
    m_applicationIdlingState->addTransition(this, &ApplicationStateMachine::applicationExit, m_applicationExitingState);
    m_applicationIdlingState->addTransition(this, &ApplicationStateMachine::jobStart, m_jobRunningState);
    m_jobRunningState->addTransition(this, &ApplicationStateMachine::jobStop, m_jobStoppingState);
    m_jobRunningState->addTransition(this, &ApplicationStateMachine::jobDone, m_applicationIdlingState);
    m_jobStoppingState->addTransition(this, &ApplicationStateMachine::jobDone, m_applicationIdlingState);

    // Connections
    connect(m_applicationStartingState, &QState::entered, this, &ApplicationStateMachine::applicationStarted);
    connect(m_applicationIdlingState, &QState::entered, this, &ApplicationStateMachine::applicationIdling);
    connect(m_applicationExitingState, &QState::entered, this, &ApplicationStateMachine::applicationExiting);
    connect(m_jobRunningState, &QState::entered, this, &ApplicationStateMachine::jobStarted);
    connect(m_jobStoppingState, &QState::entered, this, &ApplicationStateMachine::jobStopping);
    connect(m_jobDoneState, &QState::entered, this, &ApplicationStateMachine::jobFinished);

    setInitialState(m_applicationStartingState);
}

// QString ApplicationStateMachine::currentState() const
// {
//     if (configuration().contains(m_applicationStartingState))
//         return "Application started";
//     else if (configuration().contains(m_applicationIdlingState))
//         return "Application idling";
//     else if (configuration().contains(m_applicationExitingState))
//         return "Application exiting";
//     else if (configuration().contains(m_jobStartingState))
//         return "Job starting";
//     else if (configuration().contains(m_jobStoppingState))
//         return "Job stopping";
//     else if (configuration().contains(m_jobFinishingState))
//         return "Job finishing";
//     else if (configuration().contains(m_jobRunningState))
//         return "Job running";
//     else if (configuration().contains(m_jobDoneState))
//         return "Job done";
//     else
//         return "None";
// }

ApplicationState ApplicationStateMachine::currentState() const
{
    if (configuration().contains(m_applicationStartingState))
        return ApplicationState::APPLICATION_STARTED;
    else if (configuration().contains(m_applicationIdlingState))
        return ApplicationState::APPLICATION_IDLING;
    else if (configuration().contains(m_applicationExitingState))
        return ApplicationState::APPLICATION_EXITING;
    else if (configuration().contains(m_jobStartingState))
        return ApplicationState::JOB_STARTING;
    else if (configuration().contains(m_jobStoppingState))
        return ApplicationState::JOB_STOPPING;
    else if (configuration().contains(m_jobFinishingState))
        return ApplicationState::JOB_FINISHING;
    else if (configuration().contains(m_jobRunningState))
        return ApplicationState::JOB_RUNNING;
    else if (configuration().contains(m_jobDoneState))
        return ApplicationState::JOB_DONE;
    else
        return ApplicationState::NONE;
}
