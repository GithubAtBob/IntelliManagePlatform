#include "VideoWindow.h"
#include "qv3videoplayinterface.h"
#include "DataInstance.h"
#include "VideoWidget.h"
#include "treeItemInfo.h"
#include "xc_video.h"
#include "ToolInstance.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <QTimer>
#include <QMovie>
#include <QStandardItem>
#include <sstream>
#include <QMimeData>

BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam);

BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam)
{
    //EnableWindow(hWndvlc, false);
    return TRUE;
}


VideoWindow::VideoWindow(int i, QWidget *parent) : QWidget(parent), m_channelId(i), m_backImg(new QLabel(this))
{
    m_backImg->setScaledContents(true);
    m_hPlayHwnd = (HWND)winId();
	CreateActions();
    SetBackGroundPic();

    m_timer = new QTimer(this);
    StartTimer();
    connect(m_timer,SIGNAL(timeout()),SLOT(JudgeStreamSlot()));
    setMouseTracking(true);

    setAcceptDrops(true);
    /*
    parentwnd=(HWND)this->winId();
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerproc()));
    timer->start();
    */
}

void VideoWindow::timerprocSlot()
{
    //EnumChildWindows(parentwnd, EnumerateVLC, NULL);
}

void VideoWindow::JudgeStreamSlot()
{
    if(!m_isPlay)
    {
        return;
    }
    DataInstance* d = DataInstance::GetInstance();
    long ret = GetRet();
    if(ret != -1)
    {
        QV3VideoPlayInterface v3VideoPlayInterface;
        bool res = v3VideoPlayInterface.VideoCapture(ret, "D:\\IntelliManagePlatform\\debug\\pic1\\test.bmp", 0);
        if(res)
        {
            setUpdatesEnabled(false);
            m_timer->stop();
        }
    }
}


void VideoWindow::Play()
{

}

int VideoWindow::GetChannelId()
{
    return m_channelId;
}

void VideoWindow::SetChannelId(int id)
{
    m_channelId = id;
}

void VideoWindow::SetUrl(QString url)
{
	m_url = url;
}

QString VideoWindow::GetUrl()
{
	return m_url;
}

void VideoWindow::Close()
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = GetRet();
    if(ret != -1)
    {
        SetIsPlay(false);
        if(m_channelId != -1)
        {
            d->RemovehwndRetMap(m_hPlayHwnd);
            d->RemovehwndItemMap(m_hPlayHwnd);
            d->AddItemFreePool(m_channelId);
        }
        QV3VideoPlayInterface v3VideoPlayInterface;
        v3VideoPlayInterface.StopPlayVideo(ret);
        setUpdatesEnabled(true);
        StartTimer();
        ResetCursor();
        m_videoingLblFn(false);
        SetBtnState(false);
    }
    SetBackGroundPic();
}

void VideoWindow::CloseAll()
{
    m_closeAllFn();
}

void VideoWindow::Capture()
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = GetRet();
    if(ret != -1)
    {
        ToolInstance* t = ToolInstance::GetInstance();
        QV3VideoPlayInterface v3VideoPlayInterface;
        std::string path = t->RandFileName("D:\\IntelliManagePlatform\\debug\\pic1\\",".bmp");
        bool res = v3VideoPlayInterface.VideoCapture(ret, path.c_str(), 0);
    }
}

void VideoWindow::StartVideoing()
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = GetRet();
    if(ret == -1)
    {
        return;
    }
    ToolInstance* t = ToolInstance::GetInstance();
    QV3VideoPlayInterface v3VideoPlayInterface;
    std::string name = t->RandFileName("", ".mp4");
    bool res = v3VideoPlayInterface.VideoLocalRecordStart(ret, "D:\\IntelliManagePlatform\\debug\\video", name.c_str());
    if(res)
    {
        m_videoingLblFn(true);
        SetBtnState(true);
    }
}

void VideoWindow::StopVideoing()
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = GetRet();
    if(ret == -1)
    {
        return;
    }
    QV3VideoPlayInterface v3VideoPlayInterface;
    bool res = v3VideoPlayInterface.VideoLocalRecordStop(ret);
    if(res)
    {
        m_videoingLblFn(false);
        SetBtnState(false);
    }
}

void VideoWindow::SetVideoingBtn(QPushButton *btn)
{
    m_videoingBtn = btn;
}

void VideoWindow::SetBtnState(bool isVideoing)
{
    if(m_videoingBtn == nullptr)
    {
        return;
    }
    if(isVideoing)
    {
        m_videoingBtn->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118);background-color: #ffffff; color:#484848;}"
                                     "QPushButton{background-image: url(:/img/luxiang3.png); border: none;background-color:transparent;}"
                                     "QPushButton#" + m_videoingBtn->objectName() + ":hover{background-image: url(:/img/luxiang2.png); border: none;background-color:transparent;}");
    }
    else
    {
        m_videoingBtn->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118);background-color: #ffffff; color:#484848;}"
                                     "QPushButton{background-image: url(:/img/luxiang1.png); border: none;background-color:transparent;}"
                                     "QPushButton#" + m_videoingBtn->objectName() + ":hover{background-image: url(:/img/luxiang2.png); border: none;background-color:transparent;}");
    }
}

void VideoWindow::SetChangeChannelFn(std::function<void(Video::VideoControlEnum, int)> f)
{
	m_changeChannelFn = f;
}

void VideoWindow::SetFullFn(std::function<void(int idx)> f)
{
	m_fullFn = f;
}

void VideoWindow::SetCloseAllFn(std::function<void ()> f)
{
    m_closeAllFn = f;
}

void VideoWindow::SetCaptureAllFn(std::function<void ()> f)
{
    m_captureAllFn = f;
}

void VideoWindow::SetVideoingLblFn(std::function<void (bool)> f)
{
    m_videoingLblFn = f;
}

void VideoWindow::SetStartAllVideoingFn(std::function<void ()> f)
{
    m_startAllVideoingFn = f;
}

void VideoWindow::SetStopAllVideoingFn(std::function<void ()> f)
{
    m_stopAllVideoingFn = f;
}

void VideoWindow::contextMenuEvent(QContextMenuEvent * event)
{
	m_menu->clear();
	m_menu->addAction(m_switchFullNorAct);
    //m_menu->addAction(m_startPollAct);
	m_menu->addAction(m_deleteCurrentAct);
	m_menu->addAction(m_deleteAllAct);
	m_menu->addAction(m_screenshotCrtAct);
	m_menu->addAction(m_screenshotAllAct);
    m_menu->addAction(m_startVideoingCrtAct);
    m_menu->addAction(m_stopVideoingCrtAct);
    m_menu->addAction(m_startVideoingAllAct);
    m_menu->addAction(m_stopVideoingAllAct);
	
	m_menu->addMenu(m_switchFourMenu);
	m_switchFourMenu->addActions(m_switchFourActs);

	m_menu->addMenu(m_switchSixMenu);
	m_switchSixMenu->addActions(m_switchSixActs);

	m_menu->addMenu(m_switchEightMenu);
	m_switchEightMenu->addActions(m_switchEightActs);

	m_menu->addMenu(m_switchNineMenu);
	m_switchNineMenu->addActions(m_switchNineActs);

	m_menu->addAction(m_switchSixteenAct);

	//菜单出现的位置为当前鼠标的位置

	m_menu->exec(QCursor::pos());
    event->accept();
}

void VideoWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(!m_isPlay)
            return;
        DataInstance* d = DataInstance::GetInstance();
        pCMPInfo info;
        if(m_channelId == -1)
            info = d->GetItemInfo(m_hPlayHwnd, true);
        else
            info = d->GetItemInfo(m_hPlayHwnd);
        if(info->nCamType == static_cast<int>(CamTypeEnum::gun))
        {
            return;
        }
        long ret = GetRet();
        if(ret == -1)
        {
            return;
        }
        QRect  LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect;
        QPoint PT = event->pos();
        SetRect(LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect);
        QV3VideoPlayInterface v3VideoPlayInterface;
        std::stringstream type,value;
        value<<d->GetPantiltSpeed();
        bool res = false;
        if (LeftRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_LEFT;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_UP;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (RightRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_RIGHT;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (BottomRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_DOWN;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopLeftRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_FAR;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopRightRect.contains(PT))
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_NEAR;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
    }
}

void VideoWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(!m_isPlay)
            return;
        DataInstance* d = DataInstance::GetInstance();
        pCMPInfo info;
        if(m_channelId == -1)
            info = d->GetItemInfo(m_hPlayHwnd, true);
        else
            info = d->GetItemInfo(m_hPlayHwnd);
        if(info->nCamType == static_cast<int>(CamTypeEnum::gun))
        {
            return;
        }
        long ret = GetRet();
        if(ret == -1)
        {
            return;
        }
        QRect  LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect;
        QPoint PT = event->pos();
        SetRect(LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect);
        QV3VideoPlayInterface v3VideoPlayInterface;
        std::stringstream type,value;
        type<<PTZ_CMD_TYPE::PTZ_CMD_STOP;
        value<<d->GetPantiltSpeed();
        bool res = false;
        if (LeftRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (RightRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (BottomRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopLeftRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
        else if (TopRightRect.contains(PT))
        {
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
        }
    }
}

void VideoWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(!m_isPlay)
        return;
    DataInstance* d = DataInstance::GetInstance();
    pCMPInfo info;
    if(m_channelId == -1)
        info = d->GetItemInfo(m_hPlayHwnd, true);
    else
        info = d->GetItemInfo(m_hPlayHwnd);
    if(info->nCamType == static_cast<int>(CamTypeEnum::gun))
    {
        return;
    }
    long ret = GetRet();
    if(ret == -1)
    {
        return;
    }
    QRect  LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect;
    QPoint PT = event->pos();
    SetRect(LeftRect,TopRect,RightRect,BottomRect,TopLeftRect,TopRightRect);
    if (LeftRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/xiangzuo11.png")));
    }
    else if (TopRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/xiangshang11.png")));
    }
    else if (RightRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/xiangyou11.png")));
    }
    else if (BottomRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/xiangxia11.png")));
    }
    else if (TopLeftRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/suoxiao3.png")));
    }
    else if (TopRightRect.contains(PT))
    {
        setCursor(QCursor(QPixmap(":/img/fangda3.png")));
    }
    else
    {
        if(m_isMove)
            setCursor(QCursor(Qt::OpenHandCursor));
        else
            setCursor(Qt::ArrowCursor);
    }
}

void VideoWindow::mouseDoubleClickEvent(QMouseEvent * event)
{
    if(m_channelId == -1)
    {
        return;
    }
	m_fullFn(m_channelId);
    if(m_isFull)
    {
        m_switchFullNorAct->setText(tr("switchNormalAct"));
    }
    else
    {
        m_switchFullNorAct->setText(tr("switchFullAct"));
    }
}

void VideoWindow::SetSelected()
{

}

void VideoWindow::CancelSelected()
{

}

HWND VideoWindow::Gethwnd()
{
    return m_hPlayHwnd;
}

void VideoWindow::SetIsPlay(bool flag)
{
    m_isPlay = flag;
    if(m_channelId == -1)
    {
        return;
    }
    DataInstance* d = DataInstance::GetInstance();
    QStandardItem* item = d->GetItem(m_hPlayHwnd);
    if(item != nullptr)
    {
        item->data().value<TREE_ITEM_INFO*>()->isPlay = flag;
        if(item == d->GetCurrItem())
            d->LoadCamIcon(item,true);
        else
            d->LoadCamIcon(item, false);
    }
    static_cast<VideoWidget*>(parentWidget())->SetbPaint(flag);
}

void VideoWindow::SetIsFull(bool flag)
{
    m_isFull = flag;
}

bool VideoWindow::IsPlay()
{
    return m_isPlay;
}

void VideoWindow::SetBackGroundPic()
{
    QString path = ":/img/zanwushipinyulan.png";
    QPixmap pixmap(path);
    pixmap.scaled(m_backImg->size());
    m_backImg->setPixmap(pixmap);
}

void VideoWindow::SetBackGroundGif(QString path)
{
    QMovie* movie = new QMovie(path);
    movie->resized(m_backImg->size());
    m_backImg->setMovie(movie);
    movie->start();
}

void VideoWindow::StartTimer()
{
    m_timer->start(100);
}

void VideoWindow::ResetCursor()
{
    setCursor(Qt::ArrowCursor);
}

void VideoWindow::SetIsMove(bool move)
{
    m_isMove = move;
}

void VideoWindow::resizeEvent(QResizeEvent *event)
{
    m_backImg->resize(this->width()/4,this->height()/2);
    m_backImg->move((this->width()-m_backImg->width())/2,(this->height()-m_backImg->height())/2);
}

void VideoWindow::dropEvent(QDropEvent *event)
{
    DataInstance* d = DataInstance::GetInstance();
    std::string stPuid = event->mimeData()->data("stPuid").toStdString();
    std::string stChanid = event->mimeData()->data("stChanid").toStdString();
    int nFactoryCode = event->mimeData()->data("nFactoryCode").toInt();
    QString itemName = QString::fromStdString(event->mimeData()->data("itemName").toStdString());
    QStandardItem* item = d->GetItemByName(itemName);
    auto itemInfo = item->data().value<TREE_ITEM_INFO*>();
    if(itemInfo->NodeType != static_cast<int>(NodeTypeEnum::dev) || itemInfo->isPlay == true)
        return;
    pCMPInfo devData = static_cast<pCMPInfo>(itemInfo->dev);
    if(!devData->bOnline)
        return;
    if(IsPlay())
    {
        Close();
    }
    QV3VideoPlayInterface v3VideoPlayInterface;
    SetBackGroundGif(":/img/loading.gif");
    long ret = v3VideoPlayInterface.StartPlayVideo(stPuid.c_str(),stChanid.c_str(),nFactoryCode, m_hPlayHwnd);
    if(ret != -1)
    {
        d->AddhwndRetMap(m_hPlayHwnd, ret);
        d->AddhwndItemMap(m_hPlayHwnd, item);
        d->RemoveItemFreePool(m_channelId);
        SetIsPlay(true);
    }
    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void VideoWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void VideoWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void VideoWindow::CreateActions()
{
	m_menu = new QMenu();
    m_switchFullNorAct = new QAction(tr("switchFullAct"));
    //m_startPollAct = new QAction(tr("startPollAct"));
    m_deleteCurrentAct = new QAction(tr("deleteCurrentAct"));
    m_deleteAllAct = new QAction(tr("deleteAllAct"));
    m_screenshotCrtAct = new QAction(tr("screenshotCrtAct"));
    connect(m_screenshotCrtAct, &QAction::triggered, this, [this](){
        Capture();
    });
    m_screenshotAllAct = new QAction(tr("screenshotAllAct"));
    connect(m_screenshotAllAct, &QAction::triggered, this, [this](){
        m_captureAllFn();
    });
    m_startVideoingCrtAct = new QAction(tr("startVideoingCrtAct"));
    connect(m_startVideoingCrtAct, &QAction::triggered, this, [this](){
        StartVideoing();
    });
    m_stopVideoingCrtAct = new QAction(tr("stopVideoingCrtAct"));
    connect(m_stopVideoingCrtAct, &QAction::triggered, this, [this](){
        StopVideoing();
    });
    m_startVideoingAllAct = new QAction(tr("startVideoingAllAct"));
    connect(m_startVideoingAllAct, &QAction::triggered, this, [this](){
        m_startAllVideoingFn();
    });
    m_stopVideoingAllAct = new QAction(tr("stopVideoingAllAct"));
    connect(m_stopVideoingAllAct, &QAction::triggered, this, [this](){
        m_stopAllVideoingFn();
    });

    m_switchFourMenu = new QMenu(tr("switchFourMenu"));
    m_switchFourActs << new QAction(tr("switch1_4")) << new QAction(tr("switch5_8")) << new QAction(tr("switch9_12")) << new QAction(tr("switch13_16"));

    m_switchSixMenu = new QMenu(tr("switchSixMenu"));
    m_switchSixActs << new QAction(tr("switch1_6")) << new QAction(tr("switch6_11")) << new QAction(tr("switch11_16"));

    m_switchEightMenu = new QMenu(tr("switchEightMenu"));
    m_switchEightActs << new QAction(tr("switch1_8")) << new QAction(tr("switch9_16"));

    m_switchNineMenu = new QMenu(tr("switchNineMenu"));
    m_switchNineActs << new QAction(tr("switch1_9")) << new QAction(tr("switch8_16"));

    m_switchSixteenAct = new QAction(tr("switchSixteenAct"));

	connect(m_switchFullNorAct, &QAction::triggered, this, &VideoWindow::SwitchFullNorSlot);

    connect(m_deleteCurrentAct, &QAction::triggered, this, &VideoWindow::Close);
    connect(m_deleteAllAct, &QAction::triggered, this, &VideoWindow::CloseAll);

	connect(m_switchFourActs[0], &QAction::triggered, this, &VideoWindow::ChangeChannel1_4Slot);
	connect(m_switchFourActs[1], &QAction::triggered, this, &VideoWindow::ChangeChannel5_8Slot);
	connect(m_switchFourActs[2], &QAction::triggered, this, &VideoWindow::ChangeChannel9_12Slot);
	connect(m_switchFourActs[3], &QAction::triggered, this, &VideoWindow::ChangeChannel13_16Slot);

	connect(m_switchSixActs[0], &QAction::triggered, this, &VideoWindow::ChangeChannel1_6Slot);
	connect(m_switchSixActs[1], &QAction::triggered, this, &VideoWindow::ChangeChannel6_11Slot);
	connect(m_switchSixActs[2], &QAction::triggered, this, &VideoWindow::ChangeChannel11_16Slot);

	connect(m_switchEightActs[0], &QAction::triggered, this, &VideoWindow::ChangeChannel1_8Slot);
	connect(m_switchEightActs[1], &QAction::triggered, this, &VideoWindow::ChangeChannel9_16Slot);

	connect(m_switchNineActs[0], &QAction::triggered, this, &VideoWindow::ChangeChannel1_9Slot);
	connect(m_switchNineActs[1], &QAction::triggered, this, &VideoWindow::ChangeChannel8_16Slot);

    connect(m_switchSixteenAct, &QAction::triggered, this, &VideoWindow::ChangeChannel16);
}

void VideoWindow::SetRect(QRect &LeftRect, QRect &TopRect, QRect &RightRect, QRect &BottomRect, QRect &TopLeftRect, QRect &TopRightRect)
{
    int nWidth = 35;
    LeftRect.setLeft(0);
    LeftRect.setTop(nWidth*2);
    LeftRect.setRight(nWidth);
    LeftRect.setBottom(height()-nWidth*2);

    TopRect.setLeft(nWidth*2);
    TopRect.setTop(0);
    TopRect.setRight(width()-nWidth*2);
    TopRect.setBottom(nWidth);

    RightRect.setLeft(width()-nWidth);
    RightRect.setTop(nWidth*2);
    RightRect.setRight(width());
    RightRect.setBottom(height()-nWidth*2);

    BottomRect.setLeft(nWidth*2);
    BottomRect.setTop(height()-nWidth-50);
    BottomRect.setRight(width()-nWidth*2);
    BottomRect.setBottom(height()-50);

    TopLeftRect.setLeft(0);
    TopLeftRect.setTop(0);
    TopLeftRect.setRight(nWidth*2);
    TopLeftRect.setBottom(nWidth*2);

    TopRightRect.setLeft(width()-nWidth*2);
    TopRightRect.setTop(0);
    TopRightRect.setRight(width());
    TopRightRect.setBottom(nWidth*2);
}

long VideoWindow::GetRet()
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = -1;
    if(m_channelId == -1)
    {
        ret = d->GetRet(m_hPlayHwnd,true);
    }
    else
    {
        ret = d->GetRet(m_hPlayHwnd);
    }
    return ret;
}

void VideoWindow::SwitchFullNorSlot()
{
    m_fullFn(m_channelId);
    if(m_isFull)
    {
        m_switchFullNorAct->setText(tr("switchNormalAct"));
    }
    else
    {
        m_switchFullNorAct->setText(tr("switchFullAct"));
    }
}

void VideoWindow::ChangeChannel1_4Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Four, 0);
}

void VideoWindow::ChangeChannel5_8Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Four, 4);
}

void VideoWindow::ChangeChannel9_12Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Four, 8);
}

void VideoWindow::ChangeChannel13_16Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Four, 12);
}

void VideoWindow::ChangeChannel1_6Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Six, 0);
}

void VideoWindow::ChangeChannel6_11Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Six, 5);
}

void VideoWindow::ChangeChannel11_16Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Six, 10);
}

void VideoWindow::ChangeChannel1_8Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Eight, 0);
}

void VideoWindow::ChangeChannel9_16Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Eight, 8);
}

void VideoWindow::ChangeChannel1_9Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Nine, 10);
}

void VideoWindow::ChangeChannel8_16Slot()
{
	m_changeChannelFn(Video::VideoControlEnum::Nine, 7);
}

void VideoWindow::ChangeChannel16()
{
	m_changeChannelFn(Video::VideoControlEnum::Sixteen, 0);
}
