#include <QObject>
#include <QThread>

#include "thread.h"


int Thread::m_count = 0;

Thread::Thread(QObject* parent) : QThread(parent)
{
    connect(this, &Thread::started, []{
        ++Thread::m_count;
    });
    connect(this, &Thread::finished, []{
        --Thread::m_count;
    });
}

int Thread::count()
{
    return Thread::m_count;
}
