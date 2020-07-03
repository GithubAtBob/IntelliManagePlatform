#include "VideoFlowPanel.h"
#include "flowPanel.h"
#include "VideoWidget.h"
#include "VideoWindow.h"
#include <QHBoxLayout>
#include <QPushButton>

VideoFlowPanel::VideoFlowPanel(QWidget *parent) : flowPanel(parent)
{
    InitFlowPanel();
}

void VideoFlowPanel::InitFlowPanel()
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
    btns << "btnClose" << "btnSnap" << "btnVideoing" << "btnMore";
    QList<QString> tips;
    tips << "关闭" << "抓拍" << "录像" << "更多";

    QList<QString> pixmaps;
    pixmaps << (":/img/zanting1.png");
    pixmaps << (":/img/zhuapai2.png");
    pixmaps << (":/img/luxiang1.png");
    pixmaps << (":/img/gengduo1.png");

    QList<QString> hovers;
    hovers << (":/img/zanting2.png");
    hovers << (":/img/zhuapai11.png");
    hovers << (":/img/luxiang2.png");
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

        if(i==btns.count()-2)
        {
            m_prt->GetVW()->SetVideoingBtn(btn);
        }

        if(i != btns.count()-1)
        {
            layout->addWidget(btn, 0, Qt::AlignLeft);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
        }
        else
        {
            layout->addStretch();
            layout->addWidget(btn, 0, Qt::AlignRight);
            layout->addSpacing(QPixmap(pixmaps.at(i)).width());
        }
    }
}
