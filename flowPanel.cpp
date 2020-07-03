#include "flowPanel.h"
#include "VideoWidget.h"

flowPanel::flowPanel(QWidget *parent) : QWidget(parent), m_prt(static_cast<VideoWidget*>(parent))
{

}

flowPanel::~flowPanel()
{

}

/*
void flowPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient linearGradient(width(), 0, width(), height());
    linearGradient.setColorAt(0, QColor(0, 0, 0, 0));
    linearGradient.setColorAt(1, QColor(0, 0, 0, 200));
    painter.setBrush(QBrush(linearGradient));
    QPen p = painter.pen();
    p.setColor(QColor(255, 255, 255, 0));
    painter.setPen(p);
    painter.drawRect(0, 0, this->width(), this->height());
}
*/
