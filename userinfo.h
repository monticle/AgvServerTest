#ifndef USERINFO_H
#define USERINFO_H


#include <QDateTime>
#include <QString>

class UserInfo
{
public:
    UserInfo():
        id(0),
        username(""),
        password(""),
        signState(0),
        role(0),
        realname(""),
        age(0){
    }
    bool operator <(const UserInfo &b){
        return id<b.id;
    }

    UserInfo::UserInfo(const UserInfo &b)
    {
        id=b.id;
        username=b.username;
        password=b.password;
        signState=b.signState;
        lastSignTime=b.lastSignTime;
        createTime=b.createTime;
        role=b.role;
        realname=b.realname;
        age=b.age;
        sex=b.sex;
    }

    int id;
    QString username;
    QString password;
    int signState;
    QDateTime lastSignTime;
    QDateTime createTime;
    int role;
    QString realname;
    int age;
    bool sex;
};

#endif // USERINFO_H
