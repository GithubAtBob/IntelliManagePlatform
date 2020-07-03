#include "slidingvideoplaytimeline.h"
#include <QVBoxLayout>
#include <QtDebug>
#include <QLabel>
#include <QPalette>
#include <QPainter>
#include <QBrush>
#include <QDate>
#include <QToolTip>
#include <QCursor>
slidingVideoPlayTimeLine::slidingVideoPlayTimeLine(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout* vLyt = new QVBoxLayout(this);
    vLyt->setContentsMargins(10,10,10,10);

    timeLine = new VideoListTimeLine(this,25);
    vLyt->addWidget(timeLine);

    slider = new timeSlider(this);
    slider->setMaximum(24*60*60);  // 最大值
    slider->setSingleStep(1); // 步长
//    slider->setTickInterval(1);  // 设置刻度间隔
    slider->setOrientation(Qt::Horizontal);
    vLyt->addWidget(slider);

    connect(timeLine,&VideoListTimeLine::mouseDoubleClickSignal,this,[=](int sec){
        slider->setValue(sec);
    });

    connect(slider, &QSlider::valueChanged, this, [=](int num){
        double x = num;
        int curSec = static_cast<int>( x );
        QTime curTime(0, 0, 0);
        QDate curDate = QDate::currentDate();
        QDateTime curDateTime(curDate, curTime.addSecs(curSec));
        QToolTip::showText(QCursor::pos() , curDateTime.toString("yyyy.MM.dd-hh:mm:ss"), this);
    });


}


void slidingVideoPlayTimeLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(QColor(Qt::white));

    QPen p = painter.pen();
    p.setColor(QColor(255, 255, 255, 0));
    painter.setPen(p);
    painter.drawRect(0, 0, this->width(), this->height());
}
