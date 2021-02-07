#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>

class CheckUrlStatusWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit CheckUrlStatusWorker(QQueue<QMap<QString, QVariant>> &inputDataQueue, QObject *parent = nullptr);

signals:
    void result(const QMap<QString, QVariant> &resultData);
    void finished();

public slots:
    void run();

private:
    bool m_running;
    QQueue<QMap<QString, QVariant>> m_inputDataQueue;
};

