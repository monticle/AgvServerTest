#ifndef ORDER_H
#define ORDER_H

//一条指令
class Order
{
public:
    Order();
    Order(const Order &b) {
        m_rfid = b.m_rfid;
        m_cmd = b.m_cmd;
        m_param = b.m_param;
    }
    Order(int _rfid,int _cmd,int _param){
        m_rfid = _rfid;
        m_cmd = _cmd;
        m_param = _param;
    }

    void setRfid(int _rfid){m_rfid = _rfid;}
    void setCmd(int _cmd){m_cmd=_cmd;}
    void setParam(int _param){m_param=_param;}

    int rfid() const{return m_rfid;}
    int cmd() const{return m_cmd;}
    int param() const{return m_param;}

private:
    int m_rfid = 0;//卡ID
    int m_cmd = 0;//命令
    int m_param = 0;//参数
};

#endif // ORDER_H
