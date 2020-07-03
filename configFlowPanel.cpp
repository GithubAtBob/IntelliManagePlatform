#include "configFlowPanel.h"
#include "VideoWidget.h"
#include "DataInstance.h"
#include "qv3videoplayinterface.h"
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QPainter>

configFlowPanel::configFlowPanel(QWidget *parent) : QWidget(parent), m_prt(static_cast<VideoWidget*>(parent))
{
    InitConfigPanel();
}

void configFlowPanel::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    painter.setBrush(QBrush(QColor(0,0,0,150)));
//    QPen p = painter.pen();
//    p.setColor(QColor(255, 255, 255, 0));
//    painter.setPen(p);
//    painter.drawRect(0, 0, this->width(), this->height());
}

void configFlowPanel::sliderValueChangeSlot(int value)
{
    DataInstance* d = DataInstance::GetInstance();
    long ret = d->GetRet(m_prt->Gethwnd());
    if(ret == -1)
    {
        return;
    }
    QV3VideoPlayInterface v3VideoPlayInterface;
    if(value == 0)
    {
        bool res = v3VideoPlayInterface.VideoStopSound(ret);
    }
    else
    {
        bool res = v3VideoPlayInterface.VideoPlaySound(ret);
    }
}

void configFlowPanel::InitConfigPanel()
{
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
    setObjectName("configPanel");
    setVisible(false);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
//    setAttribute(Qt::WA_TranslucentBackground, true);

    QPushButton* btn = new QPushButton;
    connect(btn, SIGNAL(clicked()), m_prt, SLOT(btnClickedSlot()));
    btn->setObjectName("btnSound");
    QPixmap pixmap(":/img/shengyin1.png");
    btn->setFixedSize(pixmap.size());
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setIconSize(btn->size());
    btn->setIcon(pixmap);
    btn->setStyleSheet("border:none;background-color:transparent;");

    QSlider* slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);
    slider->setStyleSheet("border:none;background-color:transparent;");
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChangeSlot(int)));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    setLayout(layout);

    layout->addSpacing(pixmap.width());
    layout->addWidget(btn,1);
    layout->addWidget(slider,5);
    layout->addSpacing(pixmap.width());
}
