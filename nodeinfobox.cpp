#include <QDebug>
#include "nodeinfobox.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include "treeItemInfo.h"
#include "DataType.h"
#include "VideoWidget.h"
#include "VideoWindow.h"
#include "qv3videoplayinterface.h"
#include "DataInstance.h"

nodeInfoBox::nodeInfoBox(QWidget *parent) : QWidget(parent)
{
    init();
}


void nodeInfoBox::init()
{
    this->setFixedSize(480,304);
//    QWidget* topWd = new QWidget(this);

//    QPalette pll = topWd->palette();
//    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
//    topWd->setPalette(pll);
//    topWd->move(0,0);
    m_realTimeWindow = new VideoWindow(-1);
    m_realTimeWidget = new VideoWidget(m_realTimeWindow,parentWidget());
    m_replayWindow = new VideoWindow(-1);
    m_replayWidget = new VideoWidget(m_replayWindow,parentWidget(),true);

    QPixmap pixmap = QPixmap(":/img/guanbibox.png");
    closeBoxBtn = new QPushButton(QIcon(pixmap),NULL,this);
    closeBoxBtn->setStyleSheet("border:none");
    closeBoxBtn->setFixedSize(pixmap.size());
    closeBoxBtn->move(420,5);

    backgroundLab = new QLabel(this);
    backgroundLab->setFixedSize(480,254);
    backgroundLab->move(0,50);
    backgroundLab->setStyleSheet ("background-color:#FFFFFFFF");

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(682,306);
    this->raise();

    itemNameLab = new QLabel(backgroundLab);
    itemNameLab->move(28,19);
    itemNameLab->setStyleSheet("color:#333333;width:181px;height:17px;"
                               "font-size:16px;font-family:Microsoft YaHei;"
                               "font-weight:400;color:rgba(51,51,51,1);line-height:28px;");

    QLabel* organLab = new QLabel(tr("organization: "),backgroundLab);
    organLab->setStyleSheet("width:329px;height:14px;font-size:14px;font-family:Microsoft YaHei;"
                            "font-weight:400;color:rgba(51,51,51,1);line-height:28px;");
    organLab->move(28,71);

    typeLab = new QLabel(backgroundLab);
    typeLab->setStyleSheet("width:75px; height:14px;font-size:14px;"
                           "font-family:Microsoft YaHei;font-weight:400;"
                           "color:rgba(51,51,51,1);line-height:28px");
    typeLab->move(59,101);

    statLab = new QLabel(backgroundLab);
    statLab->move(59,130);
    statLab->setStyleSheet("width:75px;height:14px;font-size:14px;"
                           "font-family:Microsoft YaHei;font-weight:400;"
                           "color:rgba(51,51,51,1);line-height:28px;");

    QLabel*  applLab = new QLabel(QString("功能应用")+":  ",backgroundLab);
    applLab->move(29,160);
    applLab->setStyleSheet("width:258px;height:14px;font-size:14px;"
                           "font-family:Microsoft YaHei;font-weight:400;"
                           "color:rgba(51,51,51,1);line-height:28px;");

    collectionBtn = new QPushButton(QIcon(":/img/shoucang.png"),NULL,backgroundLab);
    ModifyLatLong = new QPushButton(QIcon(":/img/jingweidu.png"),NULL,backgroundLab);
    delLatLong = new QPushButton(QIcon(":/img/guanbijingweidu.png"),NULL,backgroundLab);

    collectionBtn->setStyleSheet("border:none");
    collectionBtn->move(342,18);

    ModifyLatLong->setStyleSheet("border:none");
    ModifyLatLong->move(390,17);

    delLatLong->setStyleSheet("border:none");
    delLatLong->move(437,17);

    PreviewBtn = new QPushButton(QIcon(":/review1.png"),"预览",backgroundLab);

    ReplayBtn = new QPushButton(QIcon(":/replay1.png"),"回放",backgroundLab);

    PreviewBtn->setFixedSize(240,44);
    PreviewBtn->move(0,210);
    PreviewBtn->setStyleSheet("width:33px;height:17px;font-size:16px;"
                              "font-family:Microsoft YaHei;font-weight:400;"
                              "color:rgba(51,51,51,1);line-height:28px;border:none");

    ReplayBtn->setFixedSize(240,44);
    ReplayBtn->move(240,210);
    ReplayBtn->setStyleSheet("width:33px;height:17px;font-size:16px;"
                             "font-family:Microsoft YaHei;font-weight:400;"
                             "color:rgba(51,51,51,1);line-height:28px;border:none");

    QLabel* lineLab = new QLabel(backgroundLab);
    lineLab->setFixedSize(426,2);
    lineLab->move(27,52);
    lineLab->setStyleSheet("background-color:#EBEFF8");

    QLabel* lineLab1 = new QLabel(backgroundLab);
    lineLab1->setFixedSize(480,2);
    lineLab1->move(0,210);
    lineLab1->setStyleSheet("background-color:blue");


    QLabel* lineLab2 = new QLabel(backgroundLab);
    lineLab2->setFixedSize(2,44);
    lineLab2->move(240,210);
    lineLab2->setStyleSheet("background-color:blue");

    //点击收藏按钮
    connect(collectionBtn,&QPushButton::clicked,this,[=](){

    });

    //点击修改经纬度按钮
    connect(ModifyLatLong,&QPushButton::clicked,this,[=](){

    });


    //点击删除经纬度按钮
    connect(delLatLong,&QPushButton::clicked,this,[=](){

    });

    //关闭按钮
    connect(closeBoxBtn,&QPushButton::clicked,this,[=](){
        this->hide();
    });

    //点击预览按钮
    connect(PreviewBtn,&QPushButton::clicked,this,[=](){
        m_realTimeWidget->show();
        m_realTimeWidget->resize(1240,600);
        m_realTimeWidget->move(627,129);
        m_realTimeWidget->raise();
        if(!m_isOnline)
            return;

        DataInstance* d = DataInstance::GetInstance();
        HWND hwnd = m_realTimeWindow->Gethwnd();
        QV3VideoPlayInterface v3VideoPlayInterface;
        m_realTimeWindow->SetBackGroundGif(":/img/loading.gif");
        long ret = v3VideoPlayInterface.StartPlayVideo(m_stPuid.toStdString().c_str(),m_stChanid.toStdString().c_str(),m_nFactoryCode, hwnd);
        if(ret != -1)
        {
            m_realTimeWindow->SetIsPlay(true);
            d->SetElemapRet(ret);
        }
    });

    //点击回放按钮
    connect(ReplayBtn,&QPushButton::clicked,this,[=](){
        this->hide();
    });

}

void nodeInfoBox::setInfo(TREE_ITEM_INFO* item)
{
    pCMPInfo devData = static_cast<pCMPInfo>(item->dev);
    itemNameLab->setText(tr("cameraname: ")+ QString::fromLocal8Bit(devData->szName));

    DataInstance* d = DataInstance::GetInstance();
    d->SetItemInfo(devData);

    m_stPuid = devData->szPuId;
    m_stChanid = QString::number(devData->nChId);
    m_nFactoryCode = devData->nFactoryCode;
    m_isOnline = devData->bOnline;

    QString type;
    if(devData->nCamType == static_cast<int>(CamTypeEnum::gun))
    {
        type = tr("gun");
    }
    else if(devData->nCamType == static_cast<int>(CamTypeEnum::hemisphere))
    {
        type = tr("hemisphere");
    }
    else
    {
        type = tr("ball");
    }
    typeLab->setText(QString("类型") +":  " + type);


    if(devData->bOnline)
    {
        statLab->setText(QString("状态：在线"));
    }
    else
    {
        statLab->setText(QString("状态：离线"));
    }
}

VideoWidget *nodeInfoBox::GetRealTimeVideoWidget()
{
    return m_realTimeWidget;
}


