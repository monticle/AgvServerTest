#include "qyhzmqconnection.h"
#include "qyhzmqconnectionworker.h"

QyhZmqConnection::QyhZmqConnection(QObject *parent) : QObject(parent)
{
    QyhZmqConnectionWorker *worker = new QyhZmqConnectionWorker;
    worker->moveToThread(&workthread);
    connect(&workthread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &QyhZmqConnection::siginit, worker, &QyhZmqConnectionWorker::init);
    connect(this, &QyhZmqConnection::reqAndRep, worker, &QyhZmqConnectionWorker::reqAndRep);
    connect(worker, &QyhZmqConnectionWorker::onRep, this, &QyhZmqConnection::onRep);
    connect(worker,&QyhZmqConnectionWorker::sendFail,this,&QyhZmqConnection::requestFail);
    workthread.start();
}

QyhZmqConnection::~QyhZmqConnection() {
    workthread.quit();
    workthread.wait();
}

void QyhZmqConnection::init(QString ip, int port)
{
    emit siginit(ip,port);
}

void QyhZmqConnection::req(QString req)
{
    emit reqAndRep(req);
}
