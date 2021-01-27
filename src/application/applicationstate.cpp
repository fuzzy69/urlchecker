#include <QStateMachine>
#include <QState>

#include "applicationstate.h"


ApplicationState::ApplicationState(QObject *parent) : QStateMachine(parent)
{
    // States
    m_startingState = new QState;
    m_idleState = new QState;
    m_exitingState = new QState;
    m_jobStartingState = new QState;
    m_jobState = new QState;
    m_jobStoppingState = new QState;
    m_jobFinishingState = new QState;

    // Transitions
    m_startingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
    m_idleState->addTransition(this, &ApplicationState::jobStarting, m_jobState);
    m_idleState->addTransition(this, &ApplicationState::applicationExiting, m_exitingState);
    m_jobState->addTransition(this, &ApplicationState::jobStopping, m_jobStoppingState);
    m_jobState->addTransition(this, &ApplicationState::jobFinishing, m_jobFinishingState);
    m_jobStoppingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
    m_jobFinishingState->addTransition(this, &ApplicationState::applicationReady, m_idleState);
//     m_startingState->assignProperty();

    //
    addState(m_startingState);
    addState(m_idleState);
    addState(m_exitingState);
    addState(m_jobStartingState);
    addState(m_jobState);
    addState(m_jobStoppingState);
    addState(m_jobFinishingState);

    //
    connect(m_startingState, &QState::entered, this, &ApplicationState::applicationStarted);
    connect(m_idleState, &QState::entered, this, &ApplicationState::applicationIdle);
    connect(m_exitingState, &QState::entered, this, &ApplicationState::applicationExit);
    connect(m_jobStartingState, &QState::entered, this, &ApplicationState::jobStarted);
    connect(m_jobState, &QState::entered, this, &ApplicationState::jobRunning);
    connect(m_jobStoppingState, &QState::entered, this, &ApplicationState::jobStopped);
    connect(m_jobFinishingState, &QState::entered, this, &ApplicationState::jobFinished);

    setInitialState(m_startingState);
//     start();
}

void ApplicationState::startingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_startingState->assignProperty(object, name, value);
}

void ApplicationState::exitingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_exitingState->assignProperty(object, name, value);
}

void ApplicationState::idleStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_idleState->assignProperty(object, name, value);
}

void ApplicationState::jobFinishingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_jobFinishingState->assignProperty(object, name, value);
}

void ApplicationState::jobStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_jobState->assignProperty(object, name, value);
}

void ApplicationState::jobStoppingStateAssignProperty(QObject* object, const char* name, const QVariant& value)
{
    m_jobStoppingState->assignProperty(object, name, value);
}
