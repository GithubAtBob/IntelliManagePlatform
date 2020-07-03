#include "VideoWidget.h"
#include "VideoWindow.h"
#include "VideoFlowPanel.h"
#include "ReplayFlowPanel.h"
#include "configFlowPanel.h"
#include "DataInstance.h"
#include "ToolInstance.h"
#include "videoingLbl.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QPushButton>

VideoWidget::VideoWidget(VideoWindow* vw, QWidget* parent, bool isReplay) : QWidget(parent), m_vw(vw),
    m_flowPanel(new flowPanel(this)), m_configPanel(new configFlowPanel(this)), m_videoingLbl(new videoingLbl(this))
{
    setMouseTracking(true);
    QHBoxLayout* lyt = new QHBoxLayout(this);
    lyt->setContentsMargins(2, 2, 2, 2);
    lyt->addWidget(vw);
    m_vw->setParent(this);
    m_vw->SetVideoingLblFn([this](bool show){
        m_videoing = show;
        if(show)
        {
            m_videoingLbl->move(width()-m_videoingLbl->width()-32,21);
            m_videoingLbl->SetIsShow(true);
        }
        else
        {
            m_videoingLbl->SetIsShow(false);
            m_videoingLbl->hide();
        }
    });
    m_videoingLbl->hide();
    if(isReplay)
    {
        delete m_flowPanel;
        m_flowPanel = new ReplayFlowPanel(this);
    }
    else
    {
        delete m_flowPanel;
        m_flowPanel = new VideoFlowPanel(this);
    }
}

void VideoWidget::SetbPaint(bool paint)
{
    m_paintFrame = paint;
    update();
}

void VideoWidget::SetVideoWin(VideoWindow *vw)
{
    m_vw = vw;
}

void VideoWidget::SetPanelVisible(bool v)
{
    m_flowPanel->setVisible(v);
    if(m_configPanelVisible || !v)
    {
        m_configPanel->setVisible(v);
        m_configPanelVisible = v;
    }
}

HWND VideoWidget::Gethwnd()
{
    return m_vw->Gethwnd();
}

VideoWindow *VideoWidget::GetVW()
{
    return m_vw;
}

void VideoWidget::btnClickedSlot()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString name = btn->objectName();
    if (name == "btnClose")
    {
        m_vw->Close();
        m_flowPanel->hide();
        m_configPanel->hide();
        if(m_vw->GetChannelId() == -1)
        {
            hide();
        }
    }
    else if(name == "btnSnap")
    {
        m_vw->Capture();
    }
    else if(name == "btnVideoing")
    {
        if(!m_videoing)
        {
            m_vw->StartVideoing();
        }
        else
        {
            m_vw->StopVideoing();
        }
    }
    else if(name == "btnMore")
    {
        if(!m_configPanelVisible)
        {
            QPalette pal(m_configPanel->palette());
            pal.setColor(QPalette::Background, "#050F2B");
            m_configPanel->setAutoFillBackground(true);
            m_configPanel->setPalette(pal);
            m_configPanel->setVisible(true);
            m_configPanel->resize(width()/3, 30);
            m_configPanel->move(width()-m_configPanel->width()-12,height()-m_configPanel->height()-55);
            m_configPanelVisible = true;
        }
        else
        {
            m_configPanel->setVisible(false);
            m_configPanelVisible = false;
        }
    }
    else if(name == "btnStop")
    {

    }
    else if(name == "btnPlay")
    {

    }
    else if(name == "btnSlow")
    {
        static_cast<ReplayFlowPanel*>(m_flowPanel)->AdjustReplaySpeed(false);
    }
    else if(name == "btnAccelerate")
    {
        static_cast<ReplayFlowPanel*>(m_flowPanel)->AdjustReplaySpeed(true);
    }
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    if(m_paintFrame)
    {
        int x = m_vw->x()-1;
        int y = m_vw->y()-1;
        int w = m_vw->width()+2;
        int h = m_vw->height()+2;
        QPainter p(this);
        p.setPen(QPen(QColor("#FF6E35"),2));
        p.drawRect(QRect(x,y,w,h));
    }
}

void VideoWidget::resizeEvent(QResizeEvent *event)
{
    m_flowPanel->resize(width()-4, 50);
    m_flowPanel->move(2,height()-52);

    m_configPanel->resize(width()/3, 30);
    m_configPanel->move(width()-m_configPanel->width()-12,height()-m_configPanel->height()-55);

    m_videoingLbl->move(width()-m_videoingLbl->width()-32,21);
}

void VideoWidget::enterEvent(QEvent *event)
{
    if(m_vw->IsPlay())
    {
        QPalette pal(m_flowPanel->palette());
        pal.setColor(QPalette::Background, "#050F2B");
        m_flowPanel->setAutoFillBackground(true);
        m_flowPanel->setPalette(pal);
        m_flowPanel->setVisible(true);
        m_flowPanel->resize(width()-4, 50);
        m_flowPanel->move(2,height()-52);
    }
}

void VideoWidget::leaveEvent(QEvent *event)
{
//    if(m_configPanel->isVisible() && m_configPanel->geometry().contains(QCursor::pos()))
//    {
//        return;
//    }
    SetPanelVisible(false);
}
