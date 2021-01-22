#include <QStateMachine>
#include <QState>

#include "applicationstate.h"


ApplicationState::ApplicationState(QStateMachine *parent) : QStateMachine(parent)
{
    // States
    m_startingState = new QState;
    m_idleState = new QState;
    m_exitingState = new QState;
    m_jobStartingState = new QState;
    m_jobStoppingState = new QState;
    m_jobFinishingState = new QState;

    // Transitions
    m_startingState->addTransition(this, &ApplicationState::applicationStarting, m_idleState);
    m_idleState->addTransition(this, &ApplicationState::jobStarting, m_jobState);
    m_idleState->addTransition(this, &ApplicationState::applicationExiting, m_exitingState);
    m_jobState->addTransition(this, &ApplicationState::jobStopping, m_jobStoppingState);
    m_jobState->addTransition(this, &ApplicationState::jobFinishing, m_jobFinishingState);
    m_jobStoppingState->addTransition(this, &ApplicationState::applicationIdling, m_idleState);
    m_jobFinishingState->addTransition(this, &ApplicationState::applicationIdling, m_idleState);

    //
    addState(m_startingState);
    addState(m_idleState);
    addState(m_exitingState);
    addState(m_jobStartingState);
    addState(m_jobState);
    addState(m_jobStoppingState);
    addState(m_jobFinishingState);

    setInitialState(m_startingState);
    start();
}
