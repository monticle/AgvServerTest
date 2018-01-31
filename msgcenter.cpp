#include "msgcenter.h"
#include "pugixml.hpp"
#include <sstream>
#include <QDebug>
#include <QCryptographicHash>
#include "qyhzmqconnection.h"
#include "global.h"

MsgCenter::MsgCenter(QObject *parent) : QObject(parent),
    queueNumber(1),
    getResponse(false),
    access_token(""),
    tcpsocket(NULL)
{
}

void MsgCenter::init()
{
    if(tcpsocket)
    {
        delete tcpsocket;
        tcpsocket = NULL;
    }
    tcpsocket = new QyhZmqConnection;
    connect(tcpsocket,SIGNAL(onRep(QString)),this,SLOT(parseOneMsg(QString)));
    tcpsocket->init("127.0.0.1",5555);

    QyhSleep(3000);
    login("qyh","1234");
}

void MsgCenter::parseSendWaitResponse(QMap<QString,QString> &requestData, QList<QMap<QString,QString> > &requestDatalist)
{
    QString xml = getRequestXml(requestData,requestDatalist);
    getResponse = false;
    qDebug() << "send:"<<xml;
    tcpsocket->reqAndRep(xml);
}

void MsgCenter::parseOneMsg(QString oneMsg)
{
    QMap<QString,QString> params;
    QList<QMap<QString,QString> > datalist;
    //解析
    qDebug() << "recv:"<<oneMsg;
    if(!getReponseParam(oneMsg,params,datalist))return ;
    QMap<QString,QString>::iterator itr;
    if((itr=params.find("type"))!=params.end()
            &&(itr=params.find("todo"))!=params.end())
    {
        if(params.contains("queuenumber") && params["queuenumber"].toInt() == queueNumber){
            getResponse = true;//收到了响应信息
        }
        //解析内容
        if(params["type"]=="user" && params["todo"]=="login")
        {
            if(params["result"]=="success"){
                //登录成功
                if(params.contains("id"))
                    current_user_info.id = (params["id"].toInt());
                if(params.contains("role"))
                    current_user_info.role = (params["role"].toInt());
                if(params.contains("age"))
                    current_user_info.age = (params["age"].toInt());
                if(params.contains("username"))
                    current_user_info.username = (params["username"]);
                if(params.contains("realname"))
                    current_user_info.realname = (params["realname"]);
                if(params.contains("password"))
                    current_user_info.password = (params["password"]);
                if(params.contains("signState"))
                    current_user_info.signState = (params["signState"].toInt());
                if(params.contains("lastSignTime"))
                    current_user_info.lastSignTime = (QDateTime::fromString(params["lastSignTime"],DATE_TIME_FORMAT));
                if(params.contains("createTime"))
                    current_user_info.createTime = (QDateTime::fromString(params["createTime"],DATE_TIME_FORMAT));

                access_token = params["access_token"];

                emit loginSuccess(current_user_info.role);
            }else{
                //登录失败，显示提示信息
                emit tip(params["info"]);
            }
        }
    }
}

bool MsgCenter::getReponseParam(const QString &xmlStr,QMap<QString,QString> &params,QList<QMap<QString,QString> > &datalist)
{
    pugi::xml_document doc;
    pugi::xml_parse_result parseResult =  doc.load_buffer(xmlStr.toStdString().c_str(), xmlStr.toStdString().length());
    if(parseResult.status != pugi::status_ok){
        //g_log->log(AGV_LOG_LEVEL_ERROR,"收到的xml解析错误:"+xmlStr);
        return false;//解析错误，说明xml格式不正确
    }

    pugi::xml_node xmlRoot = doc.child("xml");
    for (pugi::xml_node child: xmlRoot.children())
    {
        if(strcmp(child.name(),"data")!=0){
            params.insert(QString(child.name()),QString(child.child_value()));
        }else{
            for (pugi::xml_node ccchild: child.children())
            {
                if(strcmp(ccchild.name(),"datalist")!=0){
                    params.insert(QString(ccchild.name()),QString(ccchild.child_value()));
                }else{
                    for (pugi::xml_node ccccccchild: ccchild.children())
                    {
                        QMap<QString,QString> datalist_list;
                        if(strcmp(ccccccchild.name(),"list")==0){
                            for (pugi::xml_node cccccccccccccccccccchild: ccccccchild.children())
                            {
                                if(strcmp(ccccccchild.name(),"list")==0){
                                    datalist_list.insert(QString(cccccccccccccccccccchild.name()),QString(cccccccccccccccccccchild.child_value()));
                                }
                            }
                        }
                        if(datalist_list.size()>0)datalist.push_back(datalist_list);
                    }
                }
            }
        }
    }
    return true;
}

////将结果封装成xml格式(解析-封装 的封装)
QString MsgCenter::getRequestXml(QMap<QString,QString> &requestData, QList<QMap<QString,QString> > &requestDatalist)
{
    pugi::xml_document doc;
    pugi::xml_node root  = doc.append_child("xml");
    //type
    pugi::xml_node type  = root.append_child("type");
    type.text().set(requestData["type"].toStdString().c_str());

    //todo
    pugi::xml_node todo  = root.append_child("todo");
    todo.text().set(requestData["todo"].toStdString().c_str());

    //queuenumber
    if(requestData.find("queuenumber")!=requestData.end()){
        pugi::xml_node queuenumber  = root.append_child("queuenumber");
        queuenumber.text().set(requestData["queuenumber"].toStdString().c_str());
    }

    if(access_token.length()>0){
        pugi::xml_node queuenumber  = root.append_child("access_token");
        queuenumber.text().set(access_token.toStdString().c_str());
    }

    //data
    pugi::xml_node data  = root.append_child("data");
    for (QMap<QString,QString>::iterator itr=requestData.begin(); itr!=requestData.end(); ++itr)
    {
        if(itr.key() == "todo"||itr.key()=="type"||itr.key()=="queuenumber")continue;
        data.append_child(itr.key().toStdString().c_str()).text().set(itr.value().toStdString().c_str());
    }

    //datalist
    if(requestDatalist.size()>0){
        pugi::xml_node datalist  = data.append_child("datalist");
        for(QList<QMap<QString,QString> >::iterator itr=requestDatalist.begin();itr!=requestDatalist.end();++itr){
            pugi::xml_node list  = datalist.append_child("list");
            for(QMap<QString,QString>::iterator pos = itr->begin();pos!=itr->end();++pos){
                list.append_child(pos.key().toStdString().c_str()).text().set(pos.value().toStdString().c_str());
            }
        }
    }

    //封装完成
    std::stringstream result;
    doc.print(result, "", pugi::format_raw);
    return QString::fromStdString(result.str());
}


void MsgCenter::login(QString username,QString password)
{
    //1.组织xml。2.发送xml。3.等待回应 //4.结果！
    QMap<QString,QString> requestData;
    QList<QMap<QString,QString> > requestDatalist;
    requestData.insert("type","user");
    requestData.insert("todo","login");
    requestData.insert("queuenumber",QString("%1").arg(++queueNumber));

    requestData.insert("username",username);
    requestData.insert("password",password);

    parseSendWaitResponse(requestData,requestDatalist);
}


void MsgCenter::sendOrders()
{

}

void MsgCenter::stopOrders()
{

}


