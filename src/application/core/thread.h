#pragma once

#include <QThread>

class Thread : public QThread {
    Q_OBJECT

public:
    Thread(QObject* parent = nullptr);

    static int count();

private:
    static int m_count;
};
