#include "DeviceTree.h"
#include "DataInstance.h"
#include "treeItemInfo.h"
#include "qv3videoplayinterface.h"
#include "VideoControl.h"
#include "DevSortFilterProxyModel.h"
#include "VideoWidget.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QLabel>
#include <QScrollBar>

DeviceTree::DeviceTree(QWidget *parent, std::shared_ptr<VideoControl> vcptr) : QTreeView(parent),
    m_rect(new QLabel(this)), m_vcptr(vcptr)
{
    Init();
}

QStandardItem *DeviceTree::GetCurrItem()
{
    DevSortFilterProxyModel* proxyModel = static_cast<DevSortFilterProxyModel*>(this->model());
    QStandardItemModel* model = static_cast<QStandardItemModel*>(proxyModel->sourceModel());
    QModelIndex currentIndex = proxyModel->mapToSource(this->currentIndex());
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);
    return currentItem;
}

void DeviceTree::SetFn(std::function<void (TREE_ITEM_INFO *)> f)
{
    m_func = f;
}

/*
void DeviceTree::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex currentIndex = indexAt(event->pos());
    if(!currentIndex.isValid())
    {
        m_rect->hide();
        return;
    }
    int width = this->width();
    int height = sizeHintForIndex(currentIndex).height();
    int y = event->pos().y()-height/2;
    m_rect->move(0, y);
    m_rect->resize(width, height);
    m_rect->show();
    setStyleSheet("QTreeView::item:hover{background-color:#FFFFFF}"
                  "QTreeView::item:selected{background-color:#1D4ABD}"
                  "QTreeView{border: none;}");
    int delta = event->pos().y()%height;
    if(delta > height/2-5 && delta < height/2+5)
    {
        m_rect->hide();
        setStyleSheet("QTreeView::item:hover{background-color:#1D4ABD}"
                      "QTreeView::item:selected{background-color:#1D4ABD}"
                      "QTreeView{border: none;}");
    }
}
*/

void DeviceTree::Init()
{
    /*
    m_rect->setStyleSheet("background-color:#1D4ABD");
    m_rect->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    m_rect->hide();
    */
    DataInstance* d = DataInstance::GetInstance();
    QStandardItemModel* mdl = d->GetDevTreeMdl();
    DevSortFilterProxyModel* proxy = new DevSortFilterProxyModel(this);
    proxy->setSourceModel(mdl);
    setModel(proxy);
    setSortingEnabled(true);
    setHeaderHidden(true);
    expandAll();
    resize(300,610);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setIndentation(10);
    setStyleSheet("QTreeView::item:selected{background-color:#1D4ABD}"
                  "QTreeView{border: none;}");

    setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(ShowContextMenuSlot(const QPoint&)));
    //connect(this, &QTreeView::clicked, this, &DeviceTree::ClickSlot);
    connect(selectionModel(),&QItemSelectionModel::currentChanged,this,&DeviceTree::CurrentChangedSlot);
    connect(this, &QTreeView::doubleClicked, this, &DeviceTree::DoubleClickSlot);

    verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:6px;"
                                                       "background:#FFFFFF;"
                                                       "padding-top:6px;"
                                                       "padding-bottom:6px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:6px;"
                                                       "background:#1D4ABD;"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical"
                                                       "{"
                                                       "height:6px;width:8px;"
                                                       "border-image:url(:/img/xiangxia.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical"
                                                       "{"
                                                       "height:6px;width:8px;"
                                                       "border-image:url(:/img/xiangshang.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       );
    horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
                                                       "{"
                                                       "width:6px;"
                                                       "background:#FFFFFF;"
                                                       "padding-top:6px;"
                                                       "padding-bottom:6px;"
                                                       "}"
                                                       "QScrollBar::handle:horizontal"
                                                       "{"
                                                       "width:6px;"
                                                       "background:#1D4ABD;"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::add-line:horizontal"
                                                       "{"
                                                       "height:6px;width:8px;"
                                                       "border-image:url(:/img/xiangyou.png);"
                                                       "subcontrol-position:right;"
                                                       "}"
                                                       "QScrollBar::sub-line:horizontal"
                                                       "{"
                                                       "height:6px;width:8px;"
                                                       "border-image:url(:/img/xiangzuo.png);"
                                                       "subcontrol-position:left;"
                                                       "}"
                                         );
}

void DeviceTree::IncreaseChannel(int num)
{
    if(num < Video::VideoControlInt[static_cast<int>(m_vcptr->GetEnum())])
    {
        return;
    }
    if(num > 16)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Twentyfive, 0);
    }
    else if(num > 9)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Sixteen, 0);
    }
    else if(num > 8)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Nine, 0);
    }
    else if(num > 6)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Eight, 0);
    }
    else if(num > 4)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Six, 0);
    }
    else if(num > 1)
    {
        m_vcptr->SetChannel(Video::VideoControlEnum::Four, 0);
    }
}

/*
void DeviceTree::ShowContextMenuSlot(const QPoint &pos)
{
    QMenu menu;
    QAction* test = new QAction(&menu);
    QString data = QString::number(GetCurrItem()->data().value<TREE_ITEM_INFO*>()->NodeType);
    test->setText(data);
    QAction* action = new QAction(&menu);
    action->setText("删除");
    connect(action, &QAction::triggered, this, [this](){
        QStandardItemModel* model = static_cast<QStandardItemModel*>(this->model());
        QModelIndex currentIndex = this->currentIndex();
        if(!currentIndex.isValid())
            return;
        model->removeRow(currentIndex.row(), currentIndex.parent());
    });
    menu.addAction(test);
    menu.addAction(action);
    menu.exec(mapToGlobal(pos));
}
*/

/*
void DeviceTree::ClickSlot(const QModelIndex &index)
{
    DataInstance* d = DataInstance::GetInstance();
    auto currentItem = GetCurrItem();
    if(currentItem->data().value<TREE_ITEM_INFO*>()->NodeType != static_cast<int>(NodeTypeEnum::dev))
    {
        return;
    }
    d->LoadCamIcon(currentItem, true);
    if(m_lastChecked != nullptr && m_lastChecked != currentItem)
        d->LoadCamIcon(m_lastChecked, false);
    m_lastChecked = currentItem;
    d->SetCurrItem(currentItem);
}
*/

void DeviceTree::DoubleClickSlot(const QModelIndex &index)
{
    if(m_vcptr == nullptr)
    {
        auto currentItem = GetCurrItem();
        auto itemInfo = currentItem->data().value<TREE_ITEM_INFO*>();
        if(itemInfo->NodeType != static_cast<int>(NodeTypeEnum::dev))
            return;
        m_func(itemInfo);
        return;
    }

    DataInstance* d = DataInstance::GetInstance();
    int res = d->GetFirstFree();
    if(res == -1 || m_vcptr->GetWindow(res)->IsPlay())
        return;
    auto currentItem = GetCurrItem();
    auto itemInfo = currentItem->data().value<TREE_ITEM_INFO*>();
    if(itemInfo->NodeType != static_cast<int>(NodeTypeEnum::dev) || itemInfo->isPlay == true)
        return;
    pCMPInfo devData = static_cast<pCMPInfo>(itemInfo->dev);
    if(!devData->bOnline)
        return;
    QString stPuid = devData->szPuId;
    QString stChanid = QString::number(devData->nChId);
    int nFactoryCode = devData->nFactoryCode;
    HWND hwnd = m_vcptr->Gethwnd(res);
    QV3VideoPlayInterface v3VideoPlayInterface;
    m_vcptr->GetWindow(res)->SetBackGroundGif(":/img/loading.gif");
    long ret = v3VideoPlayInterface.StartPlayVideo(stPuid.toStdString().c_str(),stChanid.toStdString().c_str(),nFactoryCode, hwnd);
    if(ret != -1)
    {
        d->AddhwndRetMap(hwnd, ret);
        d->AddhwndItemMap(hwnd, currentItem);
        d->RemoveItemFreePool(res);
        m_vcptr->GetWindow(res)->SetIsPlay(true);
        IncreaseChannel(d->GetPlayingNum());
    }
}

void DeviceTree::CurrentChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    if(m_vcptr == nullptr)
        return;
    DataInstance* d = DataInstance::GetInstance();
    auto currentItem = GetCurrItem();
    if(currentItem->data().value<TREE_ITEM_INFO*>()->NodeType != static_cast<int>(NodeTypeEnum::dev))
    {
        return;
    }
    d->LoadCamIcon(currentItem, true);
    if(m_lastChecked != nullptr && m_lastChecked != currentItem)
    {
        d->LoadCamIcon(m_lastChecked, false);
    }
    m_lastChecked = currentItem;
    d->SetCurrItem(currentItem);
    auto widgets = m_vcptr->GetAllWindow();
    for(auto widget : widgets)
    {
        static_cast<VideoWidget*>(widget->parentWidget())->SetbPaint(false);
    }
    HWND hwnd = d->GetHwndByItem(currentItem);
    if(hwnd != NULL)
    {
        int idx = m_vcptr->FindByHwnd(hwnd);
        if(idx != -1)
        {
             static_cast<VideoWidget*>(m_vcptr->GetWindow(idx)->parentWidget())->SetbPaint(true);
        }
    }
}
