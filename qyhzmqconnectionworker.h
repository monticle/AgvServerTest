#ifndef QYHZMQCONNECTIONWORKER_H
#define QYHZMQCONNECTIONWORKER_H

#include <QObject>
#include <zmq.hpp>

class QyhZmqConnectionWorker : public QObject
{
    Q_OBJECT
public:
    explicit QyhZmqConnectionWorker(QObject *parent = nullptr);

signals:
    void onRep(QString s);
public slots:
    void init(QString ip,int port);
    void reqAndRep(QString s);
private:
    zmq::context_t ctx_;
    zmq::socket_t *client_socket_;
};

#endif // QYHZMQCONNECTIONWORKER_H
