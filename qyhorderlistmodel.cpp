#include "qyhorderlistmodel.h"

QyhOrderListModel::QyhOrderListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void QyhOrderListModel::insert(int index, const Order &order)
{
    if(index < 0 || index > m_list.count()) {
        return;
    }

    emit beginInsertRows(QModelIndex(), index, index);
    m_list.insert(index, order);
    emit endInsertRows();
    emit countChanged(m_list.count());
}

void QyhOrderListModel::remove(int index)
{
    if(index < 0 || index >= m_list.count()) {
        return;
    }

    emit beginRemoveRows(QModelIndex(), index, index);
    m_list.removeAt( index );
    emit endRemoveRows();
    emit countChanged(m_list.count());
}

void QyhOrderListModel::append(const Order &data)
{
    insert(count(), data);
}

void QyhOrderListModel::append(const QVariantMap map)
{
    int rfid = map["rfid"].toInt();
    int cmd = map["cmd"].toInt();
    int param = map["param"].toInt();

    Order order(rfid,cmd,param);

    insert(count(), order);
}

void QyhOrderListModel::clear()
{
    while(count()>0){
        remove(0);
    }
}


int QyhOrderListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant QyhOrderListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_list.count())
        return QVariant();

    const Order &order = m_list[index.row()];

    if (role == RfidRole)
        return order.rfid();
    else if (role == CmdRole)
        return order.cmd();
    else if (role == ParamRole)
        return order.param();

    return QVariant();
}

QHash<int, QByteArray> QyhOrderListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RfidRole] = "rfid";
    roles[CmdRole] = "cmd";
    roles[ParamRole] = "param";
    return roles;
}

int QyhOrderListModel::count() const
{
    return rowCount(QModelIndex());
}

void QyhOrderListModel::modifyRfid(int rfid,int idx)
{
    if(idx < 0 || idx >= m_list.count()) {
        return;
    }
    m_list[idx].setRfid(rfid);
    emit dataChanged(index(idx), index(idx), {0,1,2} );
}

void QyhOrderListModel::modifyCmd(int cmd,int idx)
{
    if(idx < 0 || idx >= m_list.count()) {
        return;
    }
    m_list[idx].setCmd(cmd);
    emit dataChanged(index(idx), index(idx), {0,1,2} );
}

void QyhOrderListModel::modifyParam(int param,int idx)
{
    if(idx < 0 || idx >= m_list.count()) {
        return;
    }
    m_list[idx].setParam(param);
    emit dataChanged(index(idx), index(idx), {0,1,2} );
}
