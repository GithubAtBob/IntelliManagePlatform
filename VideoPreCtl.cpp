#include "VideoPreCtl.h"
#include "DeviceTree.h"
#include "DataInstance.h"
#include "qv3videoplayinterface.h"
#include "xc_video.h"
#include "treeItemInfo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>
#include <QLabel>
#include <QButtonGroup>
#include <QSlider>
#include <sstream>

VideoPreCtl::VideoPreCtl(std::shared_ptr<VideoControl> vcptr): m_devTree(new DeviceTree(this,vcptr))
{
    InitUi();
}

DeviceTree *VideoPreCtl::GetDevTree()
{
    return m_devTree;
}

void VideoPreCtl::InitUi()
{
    setStyleSheet("background-color: #FFFFFFFF");
    m_mainLytWgt = new QWidget;
    m_mainLytWgt->setStyleSheet("background-color: #FFEBEFF7");
    QVBoxLayout* mainLyt = new QVBoxLayout(m_mainLytWgt);
    mainLyt->setContentsMargins(0, 0, 0, 0);
    QFont Font("MicrosoftYaHei", 16, QFont::Normal);
    Font.setPixelSize(16);

    m_btnBar = new QWidget;
    m_btnBar->setStyleSheet("background-color: #FFFFFF");
    QSizePolicy sizePolicy = this->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    m_btnBar->setSizePolicy(sizePolicy);
    QHBoxLayout* btnLyt = new QHBoxLayout(m_btnBar);
    btnLyt->setContentsMargins(0, 0, 0, 0);
    btnLyt->setSpacing(0);
    QPushButton* devBtn = new QPushButton("设备列表");
    SetBtnAttr(devBtn, Font);
    QPushButton* collectBtn = new QPushButton("收藏列表");
    SetBtnAttr(collectBtn, Font);
    QPushButton* hisBtn = new QPushButton("历史记录");
    SetBtnAttr(hisBtn, Font);
    connect(devBtn, &QPushButton::clicked, this, [=](){
        devBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        collectBtn->setStyleSheet("border:none;color:#777777;");
        hisBtn->setStyleSheet("border:none;color:#777777;");
    });
    connect(collectBtn, &QPushButton::clicked, this, [=](){
        collectBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        devBtn->setStyleSheet("border:none;color:#777777;");
        hisBtn->setStyleSheet("border:none;color:#777777;");
    });
    connect(hisBtn, &QPushButton::clicked, this, [=](){
        hisBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        devBtn->setStyleSheet("border:none;color:#777777;");
        collectBtn->setStyleSheet("border:none;color:#777777;");
    });

    btnLyt->addWidget(devBtn);
    btnLyt->addWidget(collectBtn);
    btnLyt->addWidget(hisBtn);

    m_deviceList = new QWidget;
    m_deviceList->setStyleSheet("background-color: #FFFFFF");
    QVBoxLayout* devLyt = new QVBoxLayout(m_deviceList);
    QLineEdit* searchEdit = new QLineEdit;
    QAction* searchAct = new QAction(searchEdit);
    searchAct->setIcon(QIcon(QPixmap(":/img/sousuo.png")));
    searchEdit->addAction(searchAct, QLineEdit::TrailingPosition);
    searchEdit->setPlaceholderText(tr("Please enter device name"));
    devLyt->addWidget(searchEdit, 32);
    devLyt->addWidget(m_devTree, 582);
    m_devTree->setDragDropMode(QAbstractItemView::DragOnly);

    m_pantiltTitle = new QWidget;
    m_pantiltTitle->setStyleSheet("background-color: #FFFFFF");
    QHBoxLayout* titleLyt = new QHBoxLayout(m_pantiltTitle);
    QLabel* lbl = new QLabel("云控制台");
    lbl->setFont(Font);
    QPushButton* hideBtn = new QPushButton;
    SetPicBtnAttr(hideBtn, ":/img/shouqi.png");
    QPushButton* popBtn = new QPushButton;
    SetPicBtnAttr(popBtn, ":/img/tanchu.png");
    popBtn->setEnabled(false);
    connect(hideBtn, &QPushButton::clicked, this, [this, hideBtn, popBtn](){
        if(!m_pantiltControl->isHidden())
        {
            m_pantiltControl->hide();
            hideBtn->setEnabled(false);
            popBtn->setEnabled(true);
        }
    });
    connect(popBtn, &QPushButton::clicked, this, [this, hideBtn, popBtn](){
        if(m_pantiltControl->isHidden())
        {
            m_pantiltControl->show();
            hideBtn->setEnabled(true);
            popBtn->setEnabled(false);
        }
    });
    titleLyt->addWidget(lbl, 109);
    titleLyt->addSpacing(15);
    titleLyt->addWidget(hideBtn, 98);
    titleLyt->addWidget(popBtn, 98);

    m_pantiltControl = new QWidget;
    m_pantiltControl->setStyleSheet("background-color: #FFFFFF");
    QVBoxLayout* panLyt = new QVBoxLayout(m_pantiltControl);
    QHBoxLayout* panbtnLyt = new QHBoxLayout;
    QWidget* lockWidget = new QWidget;
    lockWidget->setFixedSize(123,124);
    QButtonGroup* directbtnGrp = new QButtonGroup;
    QPushButton* topBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(topBtn, ":/img/shang1.png",":/img/shang.png",tr("up"));
    directbtnGrp->addButton(topBtn, static_cast<int>(DirectionEnum::TOP));
    QPushButton* lefttopBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(lefttopBtn, ":/img/zuoshang1.png",":/img/zuoshang.png",tr("leftup"));
    directbtnGrp->addButton(lefttopBtn, static_cast<int>(DirectionEnum::LEFTTOP));
    QPushButton* leftBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(leftBtn, ":/img/zuo1.png",":/img/zuo.png",tr("left"));
    directbtnGrp->addButton(leftBtn, static_cast<int>(DirectionEnum::LEFT));
    QPushButton* leftBtmBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(leftBtmBtn, ":/img/zuoxia1.png",":/img/zuoxia.png",tr("leftdown"));
    directbtnGrp->addButton(leftBtmBtn, static_cast<int>(DirectionEnum::LEFTBTM));
    QPushButton* btmBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(btmBtn, ":/img/xia1.png",":/img/xia.png",tr("down"));
    directbtnGrp->addButton(btmBtn, static_cast<int>(DirectionEnum::BTM));
    QPushButton* rightBtmBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(rightBtmBtn, ":/img/youxia1.png",":/img/youxia.png",tr("rightdown"));
    directbtnGrp->addButton(rightBtmBtn, static_cast<int>(DirectionEnum::RIGHTBTM));
    QPushButton* rightBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(rightBtn, ":/img/you1.png",":/img/you.png",tr("right"));
    directbtnGrp->addButton(rightBtn, static_cast<int>(DirectionEnum::RIGHT));
    QPushButton* rightTopBtn = new QPushButton(lockWidget);
    SetPicBtnAttr(rightTopBtn, ":/img/youshang1.png",":/img/youshang.png",tr("rightup"));
    directbtnGrp->addButton(rightTopBtn, static_cast<int>(DirectionEnum::RIGHTTOP));
    connect(directbtnGrp,SIGNAL(buttonPressed(int)),this,SLOT(DirectionPressedSlot(int)));
    connect(directbtnGrp,SIGNAL(buttonReleased(int)),this,SLOT(DirectionReleasedSlot(int)));
    QPushButton* middleBtn = new QPushButton(lockWidget);
    middleBtn->resize(QPixmap(":/img/zhengchang.png").size());
    middleBtn->setStyleSheet("border:none;");
    middleBtn->setIcon(QIcon(":/img/zhengchang.png"));
    middleBtn->setIconSize(middleBtn->size());
    connect(middleBtn, &QPushButton::clicked, this, [this,middleBtn](){
        if(m_pressed)
        {
            m_pressed = false;
            middleBtn->setIcon(QIcon(":/img/zhengchang.png"));
        }
        else
        {
            m_pressed = true;
            middleBtn->setIcon(QIcon(":/img/anxia.png"));
        }
    });
    topBtn->move(52,0);
    lefttopBtn->move(17,18);
    leftBtn->move(0,54);
    leftBtmBtn->move(17,93);
    btmBtn->move(53,114);
    rightBtmBtn->move(93,94);
    rightBtn->move(113,53);
    rightTopBtn->move(93,18);
    middleBtn->move(28,29);
    QPushButton* narBtn = new QPushButton;
    SetPicBtnAttr(narBtn, ":/img/suoxiao1.png",":/img/suoxiao2.png",tr("defocus"));
    connect(narBtn, &QPushButton::pressed, this, [this](){
        DirectionPressedSlot(static_cast<int>(DirectionEnum::NARROW));
    });
    connect(narBtn, &QPushButton::released, this, [this](){
        DirectionReleasedSlot(static_cast<int>(DirectionEnum::NARROW));
    });
    QPushButton* enLargeBtn = new QPushButton;
    SetPicBtnAttr(enLargeBtn, ":/img/fangda1.png",":/img/fangda2.png",tr("focus"));
    connect(enLargeBtn, &QPushButton::pressed, this, [this](){
        DirectionPressedSlot(static_cast<int>(DirectionEnum::ENLARGE));
    });
    connect(enLargeBtn, &QPushButton::released, this, [this](){
        DirectionReleasedSlot(static_cast<int>(DirectionEnum::ENLARGE));
    });
    panbtnLyt->addWidget(narBtn,11,Qt::AlignBottom);
    panbtnLyt->addWidget(lockWidget,19);
    panbtnLyt->addWidget(enLargeBtn,11,Qt::AlignBottom);
    QLabel* presetPos = new QLabel(tr("preset position"));
    QWidget* presetPosWidget = new QWidget;
    QVBoxLayout* presetPosLyt = new QVBoxLayout(presetPosWidget);
    QHBoxLayout* presetPosBtnLyt = new QHBoxLayout;
    QHBoxLayout* presetPosBarLyt = new QHBoxLayout;
    QButtonGroup* btnGrp = new QButtonGroup;
    QFont presetPosFont("MicrosoftYaHei", 18, QFont::Normal);
    presetPosFont.setPixelSize(18);
    for(int i=1;i<=8;++i)
    {
        QPushButton* btn = new QPushButton(QString::number(i));
        btn->setStyleSheet("border:none;");
        btn->setFont(presetPosFont);
        btnGrp->addButton(btn,i);
        presetPosBtnLyt->addWidget(btn);
    }
    connect(btnGrp,SIGNAL(buttonClicked(int)),this,SLOT(ButtonJudgeSlot(int)));
    QPushButton* speeddownBtn = new QPushButton;
    SetPicBtnAttr(speeddownBtn, ":/img/jiansu.png");
    speeddownBtn->setAutoRepeat(true);
    QSlider* speedBar = new QSlider;
    speedBar->setRange(0,10);
    speedBar->setValue(5);
    speedBar->setOrientation(Qt::Horizontal);
    speedBar->setStyleSheet("QSlider::groove:horizontal{border:1px solid #999999;height:9px;margin:0px 0;left:5px;right:5px;}"
                "QSlider::handle:horizontal{border:0px;border-image:url(:/img/biao.png);width:6px;height:10px;}"
                "QSlider::sub-page:horizontal{background-color:#1D4ABD;}");
    QPushButton* speedupBtn = new QPushButton;
    SetPicBtnAttr(speedupBtn, ":/img/jiasu.png");
    speedupBtn->setAutoRepeat(true);
    connect(speeddownBtn, &QPushButton::pressed, this, [=](){
        int v = speedBar->value();
        speedBar->setValue(--v);
    });
    connect(speedupBtn, &QPushButton::pressed, this, [=](){
        int v = speedBar->value();
        speedBar->setValue(++v);
    });
    connect(speedBar, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChangeSlot(int)));

    presetPosBarLyt->addWidget(speeddownBtn);
    presetPosBarLyt->addWidget(speedBar);
    presetPosBarLyt->addWidget(speedupBtn);
    presetPosLyt->addLayout(presetPosBtnLyt);
    presetPosLyt->addSpacing(25);
    presetPosLyt->addLayout(presetPosBarLyt);

    QWidget* panBtmWidget = new QWidget;
    QHBoxLayout* panBtmLyt = new QHBoxLayout(panBtmWidget);
    QPushButton* multifuncBtn = new QPushButton;
    multifuncBtn->resize(QPixmap(":/img/duogongneng2.png").size());
    multifuncBtn->setStyleSheet("border:none;");
    multifuncBtn->setIcon(QIcon(":/img/duogongneng2.png"));
    multifuncBtn->setIconSize(multifuncBtn->size());
    QPushButton* lightBtn = new QPushButton;
    SetPicBtnAttr(lightBtn, ":/img/dengguang1.png",":/img/dengguang2.png",tr("light"));
    lightBtn->hide();
    QPushButton* brushBtn = new QPushButton;
    SetPicBtnAttr(brushBtn, ":/img/yushua1.png",":/img/yushua2.png",tr("brush"));
    brushBtn->hide();
    QPushButton* apertureBtn = new QPushButton;
    SetPicBtnAttr(apertureBtn, ":/img/xiaoguangquan1.png",":/img/xiaoguangquan2.png",tr("aperture"));
    apertureBtn->hide();
    QPushButton* bigapertureBtn = new QPushButton;
    SetPicBtnAttr(bigapertureBtn, ":/img/daguangqun1.png",":/img/daguangquan2.png",tr("bigaperture"));
    bigapertureBtn->hide();
    QPushButton* resetPosBtn = new QPushButton;
    SetPicBtnAttr(resetPosBtn, ":/img/yuzhiwei1.png",":/img/yuzhiwei2.png",tr("resetpos"));
    resetPosBtn->hide();
    QPushButton* cruiseBtn = new QPushButton;
    SetPicBtnAttr(cruiseBtn, ":/img/xunhang1.png",":/img/xunhang2.png",tr("cruise"));
    cruiseBtn->hide();
    panBtmLyt->addWidget(multifuncBtn);
    int width = lightBtn->width();
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(lightBtn);
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(brushBtn);
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(apertureBtn);
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(bigapertureBtn);
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(resetPosBtn);
    panBtmLyt->addSpacing(width);
    panBtmLyt->addWidget(cruiseBtn);
    panBtmLyt->addSpacing(width);
    auto oldAlignment = panBtmLyt->alignment();
    panBtmLyt->setAlignment(Qt::AlignLeft);
    connect(multifuncBtn, &QPushButton::clicked, this, [=](){
        if(!m_multifuncExpand)
        {
            lightBtn->show();
            brushBtn->show();
            apertureBtn->show();
            bigapertureBtn->show();
            resetPosBtn->show();
            cruiseBtn->show();
            m_multifuncExpand = true;
            multifuncBtn->resize(QPixmap(":/img/duogongneng.png").size());
            multifuncBtn->setIcon(QIcon(":/img/duogongneng.png"));
            multifuncBtn->setIconSize(multifuncBtn->size());
            panBtmLyt->setAlignment(oldAlignment);
        }
        else
        {
            lightBtn->hide();
            brushBtn->hide();
            apertureBtn->hide();
            bigapertureBtn->hide();
            resetPosBtn->hide();
            cruiseBtn->hide();
            m_multifuncExpand = false;
            multifuncBtn->resize(QPixmap(":/img/duogongneng2.png").size());
            multifuncBtn->setIcon(QIcon(":/img/duogongneng2.png"));
            multifuncBtn->setIconSize(multifuncBtn->size());
            panBtmLyt->setAlignment(Qt::AlignLeft);
        }
    });

    panLyt->addLayout(panbtnLyt,155);
    panLyt->addSpacing(17);
    panLyt->addWidget(presetPos,20);
    panLyt->addWidget(presetPosWidget, 75);
    panLyt->addWidget(panBtmWidget, 56);

    mainLyt->setSpacing(0);
    mainLyt->addWidget(m_btnBar,67);
    mainLyt->addSpacing(1);
    mainLyt->addWidget(m_deviceList,613);
    mainLyt->addSpacing(2);
    mainLyt->addWidget(m_pantiltTitle,40);
    mainLyt->addSpacing(1);
    mainLyt->addWidget(m_pantiltControl, 300);

    QHBoxLayout* hLyt = new QHBoxLayout(this);
    hLyt->setContentsMargins(0, 0, 0, 0);
    hLyt->setSpacing(0);
    QPushButton* backBtn = new QPushButton;
    SetPicBtnAttr(backBtn, ":/img/hideCtl.png");
    QSizePolicy btnsizePolicy = backBtn->sizePolicy();
    btnsizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    backBtn->setSizePolicy(btnsizePolicy);
    connect(backBtn,&QPushButton::clicked,this, [this,backBtn](){
        if(!m_mainLytWgt->isVisible())
        {
            setMinimumWidth(320);
            m_mainLytWgt->show();
            SetPicBtnAttr(backBtn, ":/img/hideCtl.png");
        }
        else
        {
            setMinimumWidth(0);
            QSizePolicy sizePolicy = this->sizePolicy();
            sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
            setSizePolicy(sizePolicy);
            m_mainLytWgt->hide();
            SetPicBtnAttr(backBtn, ":/img/showCtl.png");
        }
    });
    hLyt->addWidget(m_mainLytWgt,300);
    hLyt->addWidget(backBtn, 20);
    setMinimumWidth(320);
}

void VideoPreCtl::SetBtnAttr(QPushButton *btn, const QFont &font)
{
    btn->setFont(font);
    btn->setStyleSheet("border:none;color:#777777;");
}

void VideoPreCtl::SetPicBtnAttr(QPushButton *btn, const QString &picPath)
{
    btn->setStyleSheet("border:none;");
    btn->setIcon(QIcon(picPath));
}

void VideoPreCtl::SetPicBtnAttr(QPushButton *btn, const QString &picPath, const QString &hoveredPath, const QString &tipStr)
{
    btn->setObjectName(tipStr);
    btn->setFixedSize(QPixmap(picPath).size());
    btn->setStyleSheet("QPushButton{background-image: url("+picPath+"); border: none;}"
                "QPushButton#" + tipStr + ":hover{background-image: url("+hoveredPath+"); border: none;}");
    btn->setToolTip(tipStr);
}

void VideoPreCtl::ButtonJudgeSlot(int idx)
{

}

void VideoPreCtl::DirectionReleasedSlot(int buttonId)
{
    DataInstance* d = DataInstance::GetInstance();
    HWND selWin = d->GetSelHwnd();
    if(selWin == NULL)
    {
        return;
    }
    pCMPInfo info = d->GetItemInfo(selWin);
    if(info->nCamType == static_cast<int>(CamTypeEnum::gun))
    {
        return;
    }
    long ret = d->GetRet(selWin);
    if(ret == -1)
    {
        return;
    }
    QV3VideoPlayInterface v3VideoPlayInterface;
    DirectionEnum idx = static_cast<DirectionEnum>(buttonId);
    std::stringstream type,value;
    type<<PTZ_CMD_TYPE::PTZ_CMD_STOP;
    value<<d->GetPantiltSpeed();
    bool res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
}

void VideoPreCtl::DirectionPressedSlot(int buttonId)
{
    DataInstance* d = DataInstance::GetInstance();
    HWND selWin = d->GetSelHwnd();
    if(selWin == NULL)
    {
        return;
    }
    pCMPInfo info = d->GetItemInfo(selWin);
    if(info->nCamType == static_cast<int>(CamTypeEnum::gun))
    {
        return;
    }
    long ret = d->GetRet(selWin);
    if(ret == -1)
    {
        return;
    }
    QV3VideoPlayInterface v3VideoPlayInterface;
    DirectionEnum idx = static_cast<DirectionEnum>(buttonId);
    std::stringstream type,value;
    value<<d->GetPantiltSpeed();
    bool res = false;
    switch (idx)
    {
        case DirectionEnum::TOP:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_UP;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::LEFTTOP:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_LEFT_UP;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::LEFT:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_LEFT;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::LEFTBTM:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_LEFT_DOWN;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::BTM:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_DOWN;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::RIGHTBTM:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_RIGHT_DOWN;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::RIGHT:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_RIGHT;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::RIGHTTOP:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_RIGHT_UP;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::NARROW:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_FAR;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        case DirectionEnum::ENLARGE:
        {
            type<<PTZ_CMD_TYPE::PTZ_CMD_NEAR;
            res = v3VideoPlayInterface.VideoPtzCtrl(ret,type.str().c_str(),value.str().c_str());
            break;
        }
        default:
            break;
    }
}

void VideoPreCtl::sliderValueChangeSlot(int value)
{
    DataInstance* d = DataInstance::GetInstance();
    d->SetPantiltSpeed(value);
}
