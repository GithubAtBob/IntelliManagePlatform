#include "ReplayFlowPanel.h"
#include "VideoWidget.h"
#include "VideoWindow.h"
#include <QPushButton>
#include <QHBoxLayout>

ReplayFlowPanel::ReplayFlowPanel(QWidget *parent) : flowPanel(parent)
{
    InitFlowPanel();
}

void ReplayFlowPanel::InitFlowPanel()
{
    //设置强焦点
    setFocusPolicy(Qt::StrongFocus);
    //设置支持拖放
    setAcceptDrops(true);
    setObjectName("flowPanel");
    setVisible(false);

    /*
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setStyleSheet("QWidget#flowPanel{border-image:url(:/img/shipinmengban.png)}");
    */

    QList<QString> btns;
    btns << "btnStop" << "btnPlay" << "btnSnap" << "btnSlow" << "btnAccelerate" << "btnMore";
    QList<QString> tips;
    tips << "暂停" << "播放" << "抓拍" << "速度调慢" << "速度调快" << "更多";

    QList<QString> pixmaps;
    pixmaps << (":/img/zanting11.png");
    pixmaps << (":/img/bofang1.png");
    pixmaps << (":/img/zhuapai1.png");
    pixmaps << (":/img/sudutiaoman.png");
    pixmaps << (":/img/sudutiaokuai.png");
    pixmaps << (":/img/gengduo1.png");

    QList<QString> hovers;
    hovers << (":/img/zanting22.png");
    hovers << (":/img/bofang2.png");
    hovers << (":/img/zhuapai11.png");
    hovers << (":/img/sudutiaoman2.png");
    hovers << (":/img/sudutiaokuai2.png");
    hovers << (":/img/gengduo2.png");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    setLayout(layout);
    //layout->setContentsMargins(0,20,0,0);

    layout->addSpacing(QPixmap(pixmaps[0]).width());

    for (int i = 0; i < btns.count(); i++)
    {
        QPushButton *btn = new QPushButton;
        connect(btn, SIGNAL(clicked()), m_prt, SLOT(btnClickedSlot()));
        btn->setObjectName(btns.at(i));
        btn->setFixedSize(QPixmap(pixmaps.at(i)).size());
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118);background-color: #ffffff; color:#484848;}"
                    "QPushButton{background-image: url("+pixmaps.at(i)+"); border: none;background-color:transparent;}"
                    "QPushButton#" + btns.at(i) + ":hover{background-image: url("+hovers.at(i)+"); border: none;background-color:transparent;}");
        btn->setToolTip(tips.at(i));

        if(i < 3)
        {
            layout->addWidget(btn, 0, Qt::AlignLeft);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
        }
        else if(i == 3)
        {
            layout->addStretch();
            layout->addWidget(btn, 0, Qt::AlignRight);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
            m_speedLbl = new QLabel;
            m_speedLbl->setText(QString::number(m_speed) + "x");
            m_speedLbl->setStyleSheet("font-size:16px;"
                                    "font-family:Microsoft YaHei;font-weight:400;color:rgba(161,161,161,1);background-color:transparent;line-height:28px");
            layout->addWidget(m_speedLbl, 0, Qt::AlignRight);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
        }
        else
        {
            layout->addWidget(btn, 0, Qt::AlignRight);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
        }
    }
}

void ReplayFlowPanel::AdjustReplaySpeed(bool fast)
{
    if((m_speed == 0.5 && !fast) || (m_speed == 2.0 && fast))
        return;
    if(fast)
    {
        m_speed += 0.5;
    }
    else
    {
        m_speed -= 0.5;
    }
    m_speedLbl->setText(QString::number(m_speed) + "x");
}
