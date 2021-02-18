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
//     m_startingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
//     m_idleState->addTransition(this, &ApplicationState::jobStarting, m_jobState);
//     m_idleState->addTransition(this, &ApplicationState::applicationExiting, m_exitingState);
//     m_jobState->addTransition(this, &ApplicationState::jobStopping, m_jobStoppingState);
//     m_jobState->addTransition(this, &ApplicationState::jobFinishing, m_jobFinishingState);
//     m_jobStoppingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
//     m_jobFinishingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);

//     m_idleState = new QState;
//     m_exitingState = new QState;
//     m_jobStartingState = new QState;
//     m_jobState = new QState;
//     m_jobStoppingState = new QState;
//     m_jobFinishingState = new QState;
// 
//     // Transitions
//     m_startingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
//     m_idleState->addTransition(this, &ApplicationState::jobStarting, m_jobState);
//     m_idleState->addTransition(this, &ApplicationState::applicationExiting, m_exitingState);
//     m_jobState->addTransition(this, &ApplicationState::jobStopping, m_jobStoppingState);
//     m_jobState->addTransition(this, &ApplicationState::jobFinishing, m_jobFinishingState);
//     m_jobStoppingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
//     m_jobFinishingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
// 
//     //
//     addState(m_startingState);
//     addState(m_idleState);
//     addState(m_exitingState);
//     addState(m_jobStartingState);
//     addState(m_jobState);
//     addState(m_jobStoppingState);
//     addState(m_jobFinishingState);
// 
    // Connections
    connect(m_applicationStartingState, &QState::entered, this, &ApplicationStateMachine::applicationStarted);
    connect(m_applicationIdlingState, &QState::entered, this, &ApplicationStateMachine::applicationIdling);
    connect(m_applicationExitingState, &QState::entered, this, &ApplicationStateMachine::applicationExiting);
    connect(m_jobRunningState, &QState::entered, this, &ApplicationStateMachine::jobStarted);
    connect(m_jobStoppingState, &QState::entered, this, &ApplicationStateMachine::jobStopping);
    connect(m_jobDoneState, &QState::entered, this, &ApplicationStateMachine::jobFinished);
//     connect(m_jobFinishingState, &QState::entered, this, &ApplicationState::jobFinished);
// 
    setInitialState(m_applicationStartingState);
}

// void ApplicationState::startingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_startingState->assignProperty(object, name, value);
// }
// 
// void ApplicationState::exitingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_exitingState->assignProperty(object, name, value);
// }
// 
// void ApplicationState::idleStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_idleState->assignProperty(object, name, value);
// }
// 
// void ApplicationState::jobFinishingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_jobFinishingState->assignProperty(object, name, value);
// }
// 
// void ApplicationState::jobStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_jobState->assignProperty(object, name, value);
// }
// 
// void ApplicationState::jobStoppingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
// {
//     m_jobStoppingState->assignProperty(object, name, value);
// }

QString ApplicationStateMachine::currentState() const
{
    if (configuration().contains(m_applicationStartingState))
        return "Application started";
    else if (configuration().contains(m_applicationIdlingState))
        return "Application idling";
    else if (configuration().contains(m_applicationExitingState))
        return "Application exiting";
    else if (configuration().contains(m_jobStartingState))
        return "Job starting";
    else if (configuration().contains(m_jobStoppingState))
        return "Job stopping";
    else if (configuration().contains(m_jobFinishingState))
        return "Job finishing";
    else if (configuration().contains(m_jobDoneState))
        return "Job done";
    else
        return "None";
}
