#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <QObject>
#include <QFile>
#include <QTimer>

#include "userinfo.h"

class QyhZmqConnection;
//处理所有的业务逻辑

class MsgCenter : public QObject
{
    Q_OBJECT
public:
    explicit MsgCenter(QObject *parent = nullptr);

    void init();

    //登录调用
    void login(QString username,QString password);

    void sendOrders();

    void stopOrders();

signals:

    //全局的 发送请求失败
    void sendRequestFail();//发送请求失败

    //全局的 等待返回超时
    void waitResponseTimeOut();//等待返回超时

    //全局的 错误提示信息
    void tip(QString tipstr);//全局的提示信息

    //全局的 登录成功
    void loginSuccess(int role);

    //任务
    void taskDetailSuccess();
public slots:

    void parseOneMsg(QString oneMsg);

private:
    ////将结果封装成xml格式(解析-封装 的封装)
    QString getRequestXml(QMap<QString,QString> &requestData, QList<QMap<QString,QString> > &requestDatalist);
    void parseSendWaitResponse(QMap<QString,QString> &requestData, QList<QMap<QString,QString> > &requestDatalist);

    QyhZmqConnection *tcpsocket;

    int queueNumber;

    volatile bool getResponse;

    QString access_token;

private:
    UserInfo current_user_info;
    bool getReponseParam(const QString &xmlStr,QMap<QString,QString> &params,QList<QMap<QString,QString> > &datalist);

    const QString DATE_TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";//统一时间格式
};

#endif // MSGCENTER_H
