#include "vWidget.h"
#include "VideoControl.h"
#include "VideoWidget.h"
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <QApplication>

vWidget::vWidget(std::shared_ptr<VideoControl> vcptr, QWidget *parent) : QWidget(parent), m_vcptr(vcptr)
{
    setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkCursorSlot()));
    timer->start(1000);
}

void vWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        setWindowFlags(Qt::SubWindow);
        showNormal();
        m_vcptr->InitLayout();
    }
}

void vWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(!isFullScreen())
        return;
    m_lastTime = QDateTime::currentDateTime();
    if (m_cursorHide)
    {
        m_cursorHide = false;
        QApplication::restoreOverrideCursor();
        if(m_currentWidget != nullptr)
        {
            m_currentWidget->SetPanelVisible(true);
        }
    }
}

void vWidget::checkCursorSlot()
{
    if(!isFullScreen())
        return;
    if (!m_cursorHide)
    {
        QDateTime now = QDateTime::currentDateTime();
        if (m_lastTime.secsTo(now) > 5)
        {
            m_cursorHide = true;
            QApplication::setOverrideCursor(Qt::BlankCursor);
            auto widgets = m_vcptr->GetAllWindow();
            for (auto widget:widgets)
            {
                if (!widget->isVisible())
                {
                    continue;
                }

                VideoWidget* vWdgt = static_cast<VideoWidget*>(widget->parentWidget());
                QPoint widgetPos = vWdgt->mapToGlobal(widget->pos());
                QRect rect(widgetPos.x(), widgetPos.y(), widget->width(), widget->height());
                if (rect.contains(QCursor::pos()))
                {
                    if(widget->IsPlay())
                    {
                        vWdgt->SetPanelVisible(false);
                        m_currentWidget = vWdgt;
                    }
                    else
                    {
                        m_currentWidget = nullptr;
                    }
                    break;
                }
            }
        }
    }
}
