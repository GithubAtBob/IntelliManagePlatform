#include "index.h"
#include "VideoSurveil.h"
#include "ElectronicMap.h"
#include "DataInstance.h"
#include "ReplayTheater.h"
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QFont>
#include <QButtonGroup>
#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QStandardItemModel>
#include <QStandardItem>

index::index(QWidget *parent)
    : QMainWindow(parent), m_widgetVec(static_cast<int>(btnIdx::num), new QWidget),
      m_mainWdgt(new QWidget), m_mainLyt(new QVBoxLayout(m_mainWdgt))
{
    LoadDataInstance();
    InitUi();
}

void index::AddWidget(btnIdx idx, QWidget *wdgt)
{
    int i = static_cast<int>(idx);
    if(i >= static_cast<int>(btnIdx::num) || i < 0)
        return;
    m_widgetVec[i] = wdgt;
}

void index::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        m_clickPos=event->pos();
}

void index::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton && m_clickPos.y()<m_titleHeight && ((m_clickPos.x()<m_maxPos&&m_clickPos.x()>m_currPos)||(m_clickPos.x()<m_LogoWeight)) && !m_double)
        move(event->pos()+pos()-m_clickPos);
}

bool index::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
        case WM_NCHITTEST:
            int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
            int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
            if(xPos < m_boundaryWidth && yPos< m_boundaryWidth)                    //左上角
                *result = HTTOPLEFT;
            else if(xPos>=width()-m_boundaryWidth&&yPos<m_boundaryWidth)          //右上角
                *result = HTTOPRIGHT;
            else if(xPos<m_boundaryWidth&&yPos>=height()-m_boundaryWidth)         //左下角
                *result = HTBOTTOMLEFT;
            else if(xPos>=width()-m_boundaryWidth&&yPos>=height()-m_boundaryWidth)//右下角
                *result = HTBOTTOMRIGHT;
            else if(xPos < m_boundaryWidth)                                     //左边
                *result =  HTLEFT;
            else if(xPos>=width()-m_boundaryWidth)                              //右边
                *result = HTRIGHT;
            else if(yPos<m_boundaryWidth)                                       //上边
                *result = HTTOP;
            else if(yPos>=height()-m_boundaryWidth)                             //下边
                *result = HTBOTTOM;
            else              //其他部分不做处理，返回false，留给其他事件处理器处理
                return false;
            return true;
    }
    return false;         //此处返回false，留给其他事件处理器处理
}

void index::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->pos().y()<m_titleHeight)
    {
        m_maxBtn->click();
        m_double = true;
    }
}

void index::mouseReleaseEvent(QMouseEvent *event)
{
    m_double = false;
}

void index::resizeEvent(QResizeEvent *size)
{
    MoveBtn();
}

void index::InitUi()
{
    m_mainLyt->setContentsMargins(0, 0, 0, 0);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    QDesktopWidget* desktopWidget =QApplication::desktop();
    QRect deskRect =desktopWidget->availableGeometry();
    int deskX = deskRect.width();
    int deskY = deskRect.height();
    setMinimumSize(1349, 759);
    setMaximumSize(deskX, deskY);
    resize(deskX, deskY);
    setStyleSheet("background-color: #EBEFF8");

    m_titleBar = new QWidget(this);
    m_titleBar->setMouseTracking(true);
    m_titleBar->resize(deskX, m_titleHeight);
    m_titleBar->setFixedHeight(m_titleHeight);
    QSizePolicy titlesizePolicy = m_titleBar->sizePolicy();
    titlesizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    m_titleBar->setSizePolicy(titlesizePolicy);
    m_titleBar->setStyleSheet("background-color: #FFFFFF");

    QLabel* logoLbl = new QLabel(m_titleBar);
    logoLbl->move(33,0);
    logoLbl->setFixedSize(m_LogoWeight, m_titleHeight);
    logoLbl->setPixmap(QPixmap(":/img/logo.png").scaled(273,31));

    QFont btnFont("MicrosoftYaHeiUI-Bold", 20, QFont::Bold);
    btnFont.setPixelSize(20);

    QPushButton* idxBtn = new QPushButton("首页", m_titleBar);
    idxBtn->setStyleSheet("border:none;");
    idxBtn->setFixedSize(180, m_titleHeight);
    idxBtn->move(383, 0);
    idxBtn->setFont(btnFont);

    QPushButton* videoPreTitle = new QPushButton("视频预览", m_titleBar);
    SetTitleBtnAttr(videoPreTitle, btnFont);

    QPushButton* videoPreClose= new QPushButton(m_titleBar);
    SetCloseBtnAttr(videoPreClose);

    QPushButton* videoBackTitle = new QPushButton("录像回放", m_titleBar);
    SetTitleBtnAttr(videoBackTitle, btnFont);

    QPushButton* videoBackClose= new QPushButton(m_titleBar);
    SetCloseBtnAttr(videoBackClose);

    QPushButton* eleMapTitle = new QPushButton("电子地图", m_titleBar);
    SetTitleBtnAttr(eleMapTitle, btnFont);

    QPushButton* eleMapClose= new QPushButton(m_titleBar);
    SetCloseBtnAttr(eleMapClose);

    QPushButton* alarmCterTitle = new QPushButton("告警中心", m_titleBar);
    SetTitleBtnAttr(alarmCterTitle,btnFont);

    QPushButton* alarmCterClose = new QPushButton(m_titleBar);
    SetCloseBtnAttr(alarmCterClose);

    QPushButton* dataCterTitle = new QPushButton("资料中心", m_titleBar);
    SetTitleBtnAttr(dataCterTitle, btnFont);

    QPushButton* dataCterClose = new QPushButton(m_titleBar);
    SetCloseBtnAttr(dataCterClose);

    QPushButton* sysConfigTitle = new QPushButton("系统配置", m_titleBar);
    SetTitleBtnAttr(sysConfigTitle, btnFont);

    QPushButton* sysConfigClose = new QPushButton(m_titleBar);
    SetCloseBtnAttr(sysConfigClose);

    QPushButton* sysLogTitle = new QPushButton("系统日志", m_titleBar);
    SetTitleBtnAttr(sysLogTitle, btnFont);

    QPushButton* sysLogClose = new QPushButton(m_titleBar);
    SetCloseBtnAttr(sysLogClose);

    m_titleGrp = new QButtonGroup(m_titleBar);
    m_titleGrp->addButton(idxBtn, static_cast<int>(btnIdx::idx));
    m_titleGrp->addButton(videoPreTitle, static_cast<int>(btnIdx::videoPre));
    m_titleGrp->addButton(videoBackTitle, static_cast<int>(btnIdx::videoBack));
    m_titleGrp->addButton(eleMapTitle,static_cast<int>(btnIdx::eleMap));
    m_titleGrp->addButton(alarmCterTitle, static_cast<int>(btnIdx::alarmCter));
    m_titleGrp->addButton(dataCterTitle, static_cast<int>(btnIdx::dataCter));
    m_titleGrp->addButton(sysConfigTitle, static_cast<int>(btnIdx::sysConfig));
    m_titleGrp->addButton(sysLogTitle, static_cast<int>(btnIdx::sysLog));
    connect(m_titleGrp,SIGNAL(buttonClicked(int)),this,SLOT(TitleButtonJudge(int)));

    m_backGrp = new QButtonGroup(m_titleBar);
    m_backGrp->addButton(videoPreClose, static_cast<int>(btnIdx::videoPre));
    m_backGrp->addButton(videoBackClose, static_cast<int>(btnIdx::videoBack));
    m_backGrp->addButton(eleMapClose,static_cast<int>(btnIdx::eleMap));
    m_backGrp->addButton(alarmCterClose, static_cast<int>(btnIdx::alarmCter));
    m_backGrp->addButton(dataCterClose, static_cast<int>(btnIdx::dataCter));
    m_backGrp->addButton(sysConfigClose, static_cast<int>(btnIdx::sysConfig));
    m_backGrp->addButton(sysLogClose, static_cast<int>(btnIdx::sysLog));
    connect(m_backGrp,SIGNAL(buttonClicked(int)),this,SLOT(BackButtonJudge(int)));

    m_remainBtn = new QPushButton("...",m_titleBar);
    m_remainBtn->setStyleSheet("border:none;image:none;");
    m_remainBtn->setFixedSize(18,18);
    m_remainBtn->hide();

    m_headLbl = new QLabel(m_titleBar);
    m_headLbl->setFixedSize(35, 34);
    m_headLbl->setPixmap(QPixmap(":/img/touxiang.png").scaled(m_headLbl->size()));

    m_unLbl = new QLabel("admin", m_titleBar);
    m_unLbl->setFixedSize(59,16);
    QFont unFont("MicrosoftYaHei", 18, QFont::Normal);
    unFont.setPixelSize(18);
    m_unLbl->setFont(unFont);

    m_dropdownBtn = new QPushButton(m_titleBar);
    m_dropdownBtn->setStyleSheet("border:none;");
    m_dropdownBtn->setIcon(QIcon(":/img/xiala.png"));
    m_dropdownBtn->setFixedSize(19,13);

    m_maxBtn = new QPushButton(m_titleBar);
    m_maxBtn->setStyleSheet("border:none;");
    m_maxBtn->setFixedSize(54, 54);
    m_maxBtn->setIcon(QIcon(":/img/fangda.png"));
    connect(m_maxBtn, &QPushButton::clicked, this, [=](){
        if(!(width()==deskX && height()==deskY))
        {
            resize(deskX, deskY);
            move(0,0);
        }
        else
        {
            resize(1349, 759);
            move((deskX-1349)/2,(deskY-759)/2);
        }
        MoveBtn();
    });

    m_minBtn = new QPushButton(m_titleBar);
    m_minBtn->setStyleSheet("border:none;");
    m_minBtn->setFixedSize(54, 54);
    m_minBtn->setIcon(QIcon(":/img/suoxiao.png"));
    connect(m_minBtn, &QPushButton::clicked, this, [this](){
        if(!isMinimized())
            showMinimized();
    });

    m_closeBtn = new QPushButton(m_titleBar);
    m_closeBtn->setStyleSheet("border:none;");
    m_closeBtn->setFixedSize(54, 54);
    m_closeBtn->setIcon(QIcon(":/img/guanbi.png"));
    connect(m_closeBtn, &QPushButton::clicked, this, [this](){
        m_videoSurveil->SaveCus();
        close();
    });

    MoveBtn();

    QWidget* homePage = new QWidget(this);
    homePage->setMouseTracking(true);
    homePage->move(0, m_titleHeight);
    homePage->resize(deskX, deskY - m_titleHeight);
    homePage->setStyleSheet("background-color: #EBEFF8");

    QFont font("MicrosoftYaHei", 20, QFont::Normal);
    font.setPixelSize(20);

    QLabel* videoModuleLbl = new QLabel("视频模块",homePage);
    videoModuleLbl->setFont(font);
    videoModuleLbl->setStyleSheet("color:#333333;");
    videoModuleLbl->setFixedSize(85, 20);
    videoModuleLbl->move(30,34);

    std::pair<int,int> idxBtnPos = std::make_pair(41,76);

    QPushButton* videoPrebtn = new QPushButton(homePage);
    SetIdxBtnAttr(videoPrebtn,idxBtnPos,":/img/shipinyulan.png", btnIdx::videoPre, "视频预览");

    QPushButton* videoBackbtn = new QPushButton(homePage);
    SetIdxBtnAttr(videoBackbtn,idxBtnPos,":/img/luxianghuifang.png", btnIdx::videoBack, "录像回放");

    QPushButton* eleMapbtn = new QPushButton(homePage);
    SetIdxBtnAttr(eleMapbtn,idxBtnPos,":/img/dianziditu.png", btnIdx::eleMap, "电子地图");

    QPushButton* alarmCterbtn = new QPushButton(homePage);
    SetIdxBtnAttr(alarmCterbtn, idxBtnPos, ":/img/gaojingzhongxin.png", btnIdx::alarmCter, "告警中心");

    QPushButton* dataCterbtn = new QPushButton(homePage);
    SetIdxBtnAttr(dataCterbtn,idxBtnPos,":/img/ziliaozhongxin.png", btnIdx::dataCter, "资料中心");

    QLabel* sysModuleLbl = new QLabel("系统模块",homePage);
    sysModuleLbl->setFont(font);
    sysModuleLbl->setFixedSize(85, 20);
    sysModuleLbl->move(30,331);

    idxBtnPos = std::make_pair(42,380);

    QPushButton* sysConfigbtn = new QPushButton(homePage);
    SetIdxBtnAttr(sysConfigbtn,idxBtnPos,":/img/xitongpeizhi.png", btnIdx::sysConfig, "系统配置");

    QPushButton* sysLogbtn = new QPushButton(homePage);
    SetIdxBtnAttr(sysLogbtn,idxBtnPos,":/img/xitongrizhi.png", btnIdx::sysLog, "系统日志");


    m_videoSurveil = new VideoSurveil(this);
    m_videoSurveil->setMouseTracking(true);
    m_videoSurveil->move(0, m_titleHeight);
    m_videoSurveil->resize(deskX, deskY - m_titleHeight);

    ElectronicMap* electronicMap = new ElectronicMap(deskX, deskY, this);
    electronicMap->setMouseTracking(true);
    electronicMap->move(0, m_titleHeight);
    electronicMap->resize(deskX, deskY - m_titleHeight);

    ReplayTheater* replaytheater = new ReplayTheater(this);
    replaytheater->setMouseTracking(true);
    replaytheater->move(0, m_titleHeight);
    replaytheater->resize(deskX, deskY - m_titleHeight);

    AddWidget(btnIdx::idx, homePage);
    AddWidget(btnIdx::videoPre, m_videoSurveil);
    AddWidget(btnIdx::eleMap, electronicMap);
    AddWidget(btnIdx::videoBack, replaytheater);
    m_titleGrp->button(0)->click();
    setCentralWidget(m_mainWdgt);
}

void index::MoveBtn()
{
    m_titleBar->resize(width(),m_titleHeight);
    m_remainBtn->move(m_titleBar->width()-465, 18);
    m_headLbl->move(m_titleBar->width()-411, 8);
    m_unLbl->move(m_titleBar->width()-364, 15);
    m_dropdownBtn->move(m_titleBar->width()-289,19);
    m_maxBtn->move(m_titleBar->width()-197, 0);
    m_minBtn->move(m_titleBar->width()-132, 0);
    m_closeBtn->move(m_titleBar->width()-65, 0);
    m_maxPos = m_titleBar->width()-465;
    //ShowHideTitleBtn();
}

void index::SetTitleBtnAttr(QPushButton* btn, QFont font)
{
    btn->setStyleSheet("border:none;");
    btn->setFixedSize(102, m_titleHeight);
    btn->setFont(font);
    btn->hide();
}

void index::SetCloseBtnAttr(QPushButton* btn)
{
    btn->setStyleSheet("border:none;");
    btn->setFixedSize(m_titleHeight, m_titleHeight);
    btn->setIcon(QIcon(":/img/tuichu.png"));
    btn->hide();
}

void index::SetIdxBtnAttr(QPushButton* btn, std::pair<int, int>& mvDist, const QString& iconPath, btnIdx idx,const QString& name)
{
    QPixmap pm(iconPath);
    QSize s = pm.size();
    btn->setStyleSheet("border:none;");
    btn->setFixedSize(s);
    btn->move(mvDist.first,mvDist.second);
    btn->setIcon(QIcon(pm));
    btn->setIconSize(btn->size());
    mvDist.first += (s.width() + s.width()/10);
    connect(btn, &QPushButton::clicked, this, [=](){
        auto titleBtn =  m_titleGrp->button(static_cast<int>(idx));
        if(titleBtn->isVisible())
        {
            m_titleGrp->button(static_cast<int>(idx))->click();
            return;
        }
        auto closeBtn = m_backGrp->button(static_cast<int>(idx));
        titleBtn->move(m_currPos,0);
        titleBtn->show();
        closeBtn->move(m_currPos + titleBtn->width(),0);
        closeBtn->show();
        m_currPos += (titleBtn->width()+closeBtn->width());
        titleBtn->click();
        //ShowHideTitleBtn();
    });
    QLabel* nameLbl = new QLabel(name, btn);
    nameLbl->setStyleSheet("background-color: #FFFFFF");
    QFont nameFont("MicrosoftYaHei", 16, QFont::Normal);
    nameFont.setPixelSize(16);
    nameLbl->setFont(nameFont);
    nameLbl->setFixedSize(68, 17);
    int moveX = (btn->width()-68)/2;
    int moveY = static_cast<int>(static_cast<double>(btn->height()-17)*0.9);
    nameLbl->move(moveX, moveY);
}

void index::ShowCurrWdgt()
{
    for(int i=0;i<static_cast<int>(btnIdx::num);++i)
    {
        if(i != m_currIdx)
            m_widgetVec[i]->hide();
        else
        {
            if(m_lastIdx != -1)
            {
                m_mainLyt->removeWidget(m_titleBar);
                m_mainLyt->removeWidget(m_widgetVec[m_lastIdx]);
            }
            m_mainLyt->setSpacing(0);
            m_mainLyt->addWidget(m_titleBar);
            m_mainLyt->addWidget(m_widgetVec[i]);
            m_widgetVec[i]->show();
        }
    }
}

void index::ShowHideTitleBtn()
{
    if(m_currPos > m_maxPos)
    {
        QMenu* menu = new QMenu();
        for(int i=1;i<static_cast<int>(btnIdx::num);++i)
        {
            auto titleBtn = m_titleGrp->button(i);
            auto closeBtn = m_backGrp->button(i);

            if(titleBtn->pos().x() > m_maxPos && m_titleGrp->button(i)->isVisible())
            {
                QAction* act = new QAction(menu);
                act->setText(titleBtn->text());
                act->setObjectName(titleBtn->text());
                menu->addAction(act);
                connect(act, &QAction::triggered, this, [=](){TitleButtonJudge(i);});

                titleBtn->setVisible(false);
                closeBtn->setVisible(false);
                int delta = titleBtn->width()+closeBtn->width();
                m_currPos -= delta;
            }

        }
        m_remainBtn->setMenu(menu);
    }
    else
    {
        for(int i=0;i<m_remainBtn->menu()->actions().count();++i)
        {
            auto titleBtn = m_titleGrp->findChild<QPushButton *>(m_remainBtn->menu()->actions().at(i)->objectName());
        }
    }
}

void index::LoadDataInstance()
{
    DataInstance* d = DataInstance::GetInstance();
    d->SetDevTreePrtPtr(this);
    d->IniDevTree();
}

/*
int index::row(QPointF pos)
{
    if(pos.y() < m_nBorder)
    {
        return 10;
    }
    else if(pos.y() > height() - m_nBorder)
    {
        return 30;
    }
    else
    {
        return 20;
    }
}

int index::col(QPointF pos)
{
    if(pos.x() < m_nBorder)
    {
        return 1;
    }
    else if(pos.x() > width() - m_nBorder)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

int index::moveArea(QPointF pos)
{
    return row(pos) + col(pos);
}

void index::setMouseStyle(int moveArea)
{
    switch (moveArea)
    {
        case 11:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case 12:
            setCursor(Qt::SizeVerCursor);
            break;
        case 13:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case 21:
            setCursor(Qt::SizeHorCursor);
            break;
        case 22:
            setCursor(Qt::ArrowCursor);
            break;
        case 23:
            setCursor(Qt::SizeHorCursor);
            break;
        case 31:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case 32:
            setCursor(Qt::SizeVerCursor);
            break;
        case 33:
            setCursor(Qt::SizeFDiagCursor);
            break;
        default:
            setCursor(Qt::WaitCursor);
            break;
    }
}
*/

void index::TitleButtonJudge(int idx)
{
    m_titleGrp->button(idx)->setStyleSheet("background-color: #EBEFF8; border:none;color:#FF1D4ABD;");
    if(idx != 0)
        m_backGrp->button(idx)->setStyleSheet("background-color: #EBEFF8; border:none;");
    for(int i=0;i < static_cast<int>(btnIdx::num); ++i)
    {
        auto titleBtn = m_titleGrp->button(i);
        if(i != idx && titleBtn->isVisible())
            titleBtn->setStyleSheet("background-color: #FFFFFF; border:none;color:#FF333333;");
    }
    for(int i=1;i < static_cast<int>(btnIdx::num); ++i)
    {
        auto closeBtn = m_backGrp->button(i);
        if(i != idx && closeBtn->isVisible())
            closeBtn->setStyleSheet("background-color: #FFFFFF; border:none;");
    }
    m_lastIdx = m_currIdx;
    m_currIdx = idx;
    ShowCurrWdgt();
}

void index::BackButtonJudge(int idx)
{
    auto titleBtn =  m_titleGrp->button(static_cast<int>(idx));
    auto closeBtn = m_backGrp->button(static_cast<int>(idx));
    int pos = titleBtn->pos().x();
    titleBtn->setVisible(false);
    closeBtn->setVisible(false);
    int delta = titleBtn->width()+closeBtn->width();
    m_currPos -= delta;
    for(int i=1;i<static_cast<int>(btnIdx::num);++i)
    {
        if(i==idx || !m_titleGrp->button(i)->isVisible())
            continue;
        int p =  m_titleGrp->button(i)->pos().x();
        if(p>pos)
        {
            int distT = p-delta;
            int distC = m_backGrp->button(i)->pos().x() -delta;
            m_titleGrp->button(i)->move(distT, 0);
            m_backGrp->button(i)->move(distC,0);
        }
    }
    if(m_currIdx == idx)
    {
        m_currIdx = 0;
        m_lastIdx = -1;
    }
    m_titleGrp->button(m_currIdx)->click();
}

