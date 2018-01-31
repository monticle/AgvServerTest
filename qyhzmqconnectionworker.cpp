#include "qyhzmqconnectionworker.h"
#include <sstream>
QyhZmqConnectionWorker::QyhZmqConnectionWorker(QObject *parent) : QObject(parent),client_socket_(NULL)
{

}

void QyhZmqConnectionWorker::init(QString ip,int port)
{
    ctx_ = zmq::context_t(1);

    if(client_socket_)
    {
        delete client_socket_;
        client_socket_ = NULL;
    }
    client_socket_ = new zmq::socket_t(ctx_,ZMQ_REQ);

    int timeout = 1000;
    zmq_setsockopt (client_socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    zmq_setsockopt (client_socket_, ZMQ_SNDTIMEO, &timeout, sizeof(timeout));

    //  Configure socket to not wait at close time
    int linger = 0;
    client_socket_->setsockopt (ZMQ_LINGER, &linger, sizeof (linger));

    std::string portstr;
    std::stringstream ssss;
    ssss<<port;
    ssss>>portstr;

    std::string ss = "tcp://"+ip.toStdString()+":"+portstr;

    client_socket_->connect(ss.c_str());
}

void QyhZmqConnectionWorker::reqAndRep(QString s)
{
    try{
        zmq::message_t msg(s.toStdString().c_str(),s.toStdString().length());
        client_socket_->send(msg);
        zmq::message_t m;
        client_socket_->recv(&m);
        std::string rep = std::string((char *)m.data(),m.size());
        emit onRep(QString::fromStdString(rep));
    }
    catch(std::exception e){}
}
