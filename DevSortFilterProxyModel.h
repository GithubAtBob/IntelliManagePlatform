#ifndef DEVSORTFILTERPROXY_H
#define DEVSORTFILTERPROXY_H

#include <QSortFilterProxyModel>

class DevSortFilterProxyModel: public QSortFilterProxyModel
{
public:
    DevSortFilterProxyModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QMimeData* mimeData(const QModelIndexList &indexes) const;

protected:
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};

#endif // DEVSORTFILTERPROXY_H
