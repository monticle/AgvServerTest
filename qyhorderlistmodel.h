#ifndef QYHORDERLISTMODEL_H
#define QYHORDERLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "order.h"

class QyhOrderListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum OrderRoles
    {
        RfidRole = Qt::UserRole + 1,
        CmdRole,
        ParamRole
     };
    QyhOrderListModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void insert(int index, const Order &order);
    Q_INVOKABLE void append(const Order &data);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void append(const QVariantMap map);

    Q_INVOKABLE void clear();

    Q_INVOKABLE void modifyRfid(int rfid,int idx);
    Q_INVOKABLE void modifyCmd(int cmd,int idx);
    Q_INVOKABLE void modifyParam(int param,int idx);

    QList<Order> getDataList(){return m_list;}
signals:
    void countChanged(int arg);
private:
    int count() const;
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Order> m_list;
};

#endif // QYHORDERLISTMODEL_H
