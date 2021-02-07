#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQueue>

#include "checkurlstatusworker.h"

CheckUrlStatusWorker::CheckUrlStatusWorker(QQueue<QMap<QString, QVariant> >& inputDataQueue, QObject* parent) :
    QObject(parent), m_running(false), m_inputDataQueue(inputDataQueue)
{
    
}

void CheckUrlStatusWorker::run()
{
    m_running = true;
    while (!m_inputDataQueue.empty())
    {
        auto inputData = m_inputDataQueue.dequeue();
        qDebug() << 1;
        QThread::sleep(2);
        auto data = QMap<QString, QVariant>{
            {QString("rowId"), QVariant(inputData["rowId"].toInt())},
            {QString("status"), QVariant(200)},
            {QString("message"), QVariant("OK")},
            {QString("url"), QVariant(inputData["url"].toString())}
        };
        emit result(data);
        QThread::sleep(2);
        qDebug() << 2;
    }
//     qDebug() << 1;
//     QThread::sleep(2);
//     auto data = QMap<QString, QVariant>{
//         {QString("rowId"), QVariant(1)},
//         {QString("status"), QVariant(200)},
//         {QString("message"), QVariant("OK")}
//     };
// //     emit result(QString("OK"));
//     emit result(data);
//     QThread::sleep(2);
//     qDebug() << 2;

    emit finished();
}
