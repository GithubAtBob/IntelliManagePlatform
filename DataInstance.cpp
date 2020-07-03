#include "DataInstance.h"
#include "treeItemInfo.h"
#include "qglobaldata.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QStandardItem>

DataInstance* DataInstance::me = NULL;

DataInstance* DataInstance::GetInstance()
{
    if(me == NULL)
    {
        me = new DataInstance;
    }

    return me;
}

QStandardItemModel *DataInstance::GetDevTreeMdl()
{
    return m_devTreeMdl;
}

void DataInstance::SetDevTreePrtPtr(QWidget *prtPtr)
{
    m_devTreeMdl->setParent(prtPtr);
}

DataInstance::DataInstance() : m_devTreeMdl(new QStandardItemModel)
{

}

void DataInstance::AddItem(QStandardItem *item, QStandardItem *parent)
{
    if(parent == nullptr)
        m_devTreeMdl->appendRow(item);
    else
        parent->appendRow(item);
}

void DataInstance::SetIcon(QStandardItem *item, const QString &iconPath)
{
    if(iconPath == "")
        return;
    item->setIcon(QIcon(iconPath));
}

void DataInstance::SetData(QStandardItem *item, TREE_ITEM_INFO *itemInfo)
{
    QVariant v;
    v.setValue(itemInfo);
    item->setData(v);
}

void DataInstance::IniNode(QStandardItem *item, QStandardItem *parent, const QString &iconPath, TREE_ITEM_INFO *itemInfo)
{
    SetIcon(item, iconPath);
    SetData(item, itemInfo);
    AddItem(item, parent);
}

void DataInstance::IniDevTree()
{
    QGlobalData *pGlobalData = QGlobalData::GetInstance();
    pGlobalData->GetAllMpInfo();
    QStandardItem* itemSys = new QStandardItem("All Device");
    TREE_ITEM_INFO* info = new TREE_ITEM_INFO;
    info->NodeType = static_cast<int>(NodeTypeEnum::root);
    IniNode(itemSys, nullptr, ":/img/xuanzhong.png", info);
    for(int i=0;i<pGlobalData->chancount;i++)
    {
        pCMPInfo mpinfo=pGlobalData->chanlist[i];
//        if(!mpinfo->bOnline)
//            continue;
        if(mpinfo)
        {
            QStandardItem* itemVideo = new QStandardItem(QString::fromLocal8Bit(mpinfo->szName));
            TREE_ITEM_INFO* info = new TREE_ITEM_INFO;
            info->NodeType = static_cast<int>(NodeTypeEnum::dev);
            info->dev = mpinfo;
            IniNode(itemVideo, itemSys, "", info);
            LoadCamIcon(itemVideo, false);
        }
    }
    pGlobalData->Destroy();

    /*
    QStandardItem* itemSys = new QStandardItem("System");
    TREE_ITEM_INFO* info = new TREE_ITEM_INFO;
    IniNode(itemSys, nullptr, ":/img/fangda.png", info);
    for(int i=0;i<30;++i)
    {
        QStandardItem* itemGrp = new QStandardItem( "Group");
        TREE_ITEM_INFO* info = new TREE_ITEM_INFO;
        IniNode(itemGrp, itemSys, ":/img/suoxiao.png", info);
        for(int j=0;j<4;++j)
        {
            QStandardItem* itemVideo = new QStandardItem("Video"+QString::number(j));
            TREE_ITEM_INFO* info = new TREE_ITEM_INFO;
            IniNode(itemVideo, itemGrp, ":/img/guanbi.png", info);
        }
    }
    */
}

void DataInstance::LoadCamIcon(QStandardItem *item, bool isChecked)
{
    int camType = static_cast<pCMPInfo>(item->data().value<TREE_ITEM_INFO*>()->dev)->nCamType;
    bool s = static_cast<pCMPInfo>(item->data().value<TREE_ITEM_INFO*>()->dev)->bOnline;
    CamStatusEnum status;
    if(item->data().value<TREE_ITEM_INFO*>()->isPlay)
    {
        status = CamStatusEnum::playing;
    }
    else
    {
        if(s)
            status = CamStatusEnum::online;
        else
            status = CamStatusEnum::offline;
    }
    if(camType == static_cast<int>(CamTypeEnum::gun))
    {
        if(status == CamStatusEnum::online)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qinagjilan.png");
            else
                SetIcon(item, ":/img/qinagjibai.png");
        }
        else if(status == CamStatusEnum::offline)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qinagjilan2.png");
            else
                SetIcon(item, ":/img/qinagjilanbai2.png");
        }
        else if(status == CamStatusEnum::playing)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qinagjilan3.png");
            else
                SetIcon(item, ":/img/qinagjibai3.png");
        }
    }
    else if(camType == static_cast<int>(CamTypeEnum::hemisphere))
    {
        if(status == CamStatusEnum::online)
        {
            if(!isChecked)
                SetIcon(item, ":/img/banqiujilan.png");
            else
                SetIcon(item, ":/img/banqiujibai.png");
        }
        else if(status == CamStatusEnum::offline)
        {
            if(!isChecked)
                SetIcon(item, ":/img/banqiujilan2.png");
            else
                SetIcon(item, ":/img/banqiujilanbai2.png");
        }
        else if(status == CamStatusEnum::playing)
        {
            if(!isChecked)
                SetIcon(item, ":/img/banqiujilan3.png");
            else
                SetIcon(item, ":/img/banqiujibai3.png");
        }
    }
    else
    {
        if(status == CamStatusEnum::online)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qiujilan.png");
            else
                SetIcon(item, ":/img/qiujibai.png");
        }
        else if(status == CamStatusEnum::offline)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qiujilan2.png");
            else
                SetIcon(item, ":/img/qiujilanbai2.png");
        }
        else if(status == CamStatusEnum::playing)
        {
            if(!isChecked)
                SetIcon(item, ":/img/qiujilan3.png");
            else
                SetIcon(item, ":/img/qiujibai3.png");
        }
    }
}

void DataInstance::SetCurrItem(QStandardItem *item)
{
    m_currItem = item;
}

QStandardItem *DataInstance::GetCurrItem()
{
    return m_currItem;
}

QStandardItem *DataInstance::GetItemByName(const QString &name)
{
    return m_devTreeMdl->findItems(name,Qt::MatchExactly|Qt::MatchRecursive).at(0);
}

void DataInstance::AddhwndRetMap(HWND hwnd, long ret)
{
    std::map<HWND, long>::iterator itFind = m_hwndRetMap.find(hwnd);
    if(itFind==m_hwndRetMap.end())
        m_hwndRetMap.insert(std::make_pair(hwnd,ret));
}

long DataInstance::GetRet(HWND hwnd, bool isElemap)
{
    if(isElemap)
    {
        return m_electronicMapRet;
    }
    auto i = m_hwndRetMap.find(hwnd);
    if(i != m_hwndRetMap.end())
        return i->second;
    return -1;
}

void DataInstance::SetElemapRet(long ret)
{
    m_electronicMapRet = ret;
}

std::vector<long> DataInstance::GetAllRet()
{
    std::vector<long> ret;
    for(auto i : m_hwndRetMap)
    {
        ret.push_back(i.second);
    }
    return ret;
}

void DataInstance::RemovehwndRetMap(HWND hwnd)
{
    auto i = m_hwndRetMap.find(hwnd);
    m_hwndRetMap.erase(i);
}

void DataInstance::ClearhwndRetMap()
{
    m_hwndRetMap.clear();
}

int DataInstance::GetPlayingNum()
{
    return m_hwndRetMap.size();
}

void DataInstance::IniFreePool(int size)
{
    for(int i=0;i<size;++i)
    {
        m_freePool.insert(i);
    }
}

void DataInstance::RemoveItemFreePool(int idx)
{
    for(auto i=m_freePool.begin();i!=m_freePool.end();++i)
    {
        if(*i == idx)
        {
            m_freePool.erase(i);
            break;
        }
    }
}

void DataInstance::AddItemFreePool(int idx)
{
    m_freePool.insert(idx);
}

int DataInstance::GetFirstFree()
{
    if(m_freePool.begin()!=m_freePool.end())
       return *m_freePool.begin();
    return -1;
}

void DataInstance::AddhwndItemMap(HWND hwnd, QStandardItem *item)
{
    m_hwndItemMap[hwnd] = item;
}

void DataInstance::RemovehwndItemMap(HWND hwnd)
{
    auto i = m_hwndItemMap.find(hwnd);
    m_hwndItemMap.erase(i);
}

QStandardItem *DataInstance::GetItem(HWND hwnd)
{
    auto i = m_hwndItemMap.find(hwnd);
    if(i != m_hwndItemMap.end())
        return i->second;
    return nullptr;
}

pCMPInfo DataInstance::GetItemInfo(HWND hwnd, bool isElemap)
{
    if(isElemap)
    {
        return m_elemapItemInfo;
    }
    QStandardItem* item = GetItem(hwnd);
    pCMPInfo info = static_cast<pCMPInfo>(item->data().value<TREE_ITEM_INFO*>()->dev);
    return info;
}

void DataInstance::SetItemInfo(pCMPInfo info)
{
    m_elemapItemInfo = info;
}

HWND DataInstance::GetHwndByItem(QStandardItem *item)
{
    auto find_item = std::find_if(m_hwndItemMap.begin(), m_hwndItemMap.end(),
        [item](const std::map<HWND, QStandardItem*>::value_type i)
    {
        return i.second == item;
    });

    HWND res = NULL;
    if (find_item!= m_hwndItemMap.end())
    {
        res = (*find_item).first;
    }
    return res;
}

void DataInstance::ClearhwndItemMap()
{
    m_hwndItemMap.clear();
}

void DataInstance::SetSelHwnd(HWND hwnd)
{
    m_selPlayingHwnd = hwnd;
}

HWND DataInstance::GetSelHwnd()
{
    return m_selPlayingHwnd;
}

void DataInstance::SetPantiltSpeed(int speed)
{
    m_pantiltSpeed = speed;
}

int DataInstance::GetPantiltSpeed()
{
    return m_pantiltSpeed;
}
