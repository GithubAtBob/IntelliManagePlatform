#include "videoingLbl.h"
#include <QLabel>
#include <QTimer>

videoingLbl::videoingLbl(QWidget *parent) : QLabel(parent)
{
    InitVideoingLbl();
    QTimer* timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),SLOT(TwinkleSlot()));
}

void videoingLbl::SetIsShow(bool isShow)
{
    m_isShow = isShow;
}

void videoingLbl::InitVideoingLbl()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    QString path = ":/img/luxiangzhjong.png";
    QPixmap pixmap(path);
    setPixmap(pixmap);
    setFixedSize(pixmap.size());
}

void videoingLbl::TwinkleSlot()
{
    if(m_isShow)
    {
        if(m_isTwinkle)
        {
            m_isTwinkle = false;
            show();
        }
        else
        {
            m_isTwinkle = true;
            hide();
        }
    }
}
