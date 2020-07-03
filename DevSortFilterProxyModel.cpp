#include "DevSortFilterProxyModel.h"
#include "treeItemInfo.h"
#include "DataType.h"
#include <QStandardItemModel>
#include <QMimeData>

DevSortFilterProxyModel::DevSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

Qt::ItemFlags DevSortFilterProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags = flags | Qt::ItemIsDragEnabled;
    return flags;
}

QMimeData *DevSortFilterProxyModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.count() <= 0)
        return nullptr;
    QMimeData *data = new QMimeData;
    QStandardItemModel* model = static_cast<QStandardItemModel*>(sourceModel());
    QModelIndex index = mapToSource(indexes.at(0));
    QStandardItem* item = model->itemFromIndex(index);
    TREE_ITEM_INFO* info = item->data().value<TREE_ITEM_INFO*>();
    if(info->NodeType != static_cast<int>(NodeTypeEnum::dev) || info->isPlay == true)
        return nullptr;
    pCMPInfo devinfo = static_cast<pCMPInfo>(info->dev);
    data->setData("stPuid", devinfo->szPuId);
    data->setData("stChanid",QByteArray::number(devinfo->nChId));
    data->setData("nFactoryCode",QByteArray::number(devinfo->nFactoryCode));
    data->setData("itemName",item->text().toStdString().c_str());
    return data;
}

bool DevSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    pCMPInfo l = static_cast<pCMPInfo>(static_cast<QStandardItemModel*>(sourceModel())
                                       ->itemFromIndex(left)->data().value<TREE_ITEM_INFO*>()->dev);
    pCMPInfo r = static_cast<pCMPInfo>(static_cast<QStandardItemModel*>(sourceModel())
                                       ->itemFromIndex(right)->data().value<TREE_ITEM_INFO*>()->dev);
    if(l->bOnline && !r->bOnline)
    {
        return false;
    }
    else if(!l->bOnline && r->bOnline)
    {
        return true;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}
