#ifndef QYHZMQCONNECTION_H
#define QYHZMQCONNECTION_H

#include <QObject>
#include <QThread>

class QyhZmqConnection : public QObject
{
    Q_OBJECT

    QThread workthread;
public:
    QyhZmqConnection(QObject *parent = nullptr);
    ~QyhZmqConnection();
    void init(QString ip, int port);
    void req(QString req);
signals:
    void siginit(QString ip,int port);
    void reqAndRep(QString req);
    void onRep(QString rep);
public slots:

};

#endif // QYHZMQCONNECTION_H
