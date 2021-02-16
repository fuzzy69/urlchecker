#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QQueue>


class Worker : public QObject
{
    Q_OBJECT
    
public:
    explicit Worker(QQueue<QMap<QString, QVariant>> &inputDataQueue, QObject *parent = nullptr);

signals:
    void result(const QMap<QString, QVariant> &resultData);
    void finished();
    void requestStop();

public slots:
    virtual void run();
    void stop();

protected:
    bool m_running;
    QQueue<QMap<QString, QVariant>> m_inputDataQueue;
};

