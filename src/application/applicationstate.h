#pragma once

#include <QStateMachine>


class QState;

class ApplicationState : public QStateMachine
{
    Q_OBJECT

public:
    ApplicationState(QStateMachine *parent = nullptr);

signals:
    void applicationStarting();
    void applicationIdling();
    void applicationExiting();
    void jobStarting();
    void jobStopping();
    void jobFinishing();


private:
    QState *m_startingState;
    QState *m_idleState;
    QState *m_exitingState;
    QState *m_jobStartingState;
    QState *m_jobState;
    QState *m_jobStoppingState;
    QState *m_jobFinishingState;
};
