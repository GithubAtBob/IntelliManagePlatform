#include "VideoSurveil.h"
#include "VideoControl.h"
#include "VideoPreCtl.h"

VideoSurveil::VideoSurveil(QWidget *parent)
    : QWidget(parent), m_mainLayout(new QHBoxLayout(this))
{
    auto vcptr = std::make_shared<VideoControl>();
    m_videoPreCtl = new VideoPreCtl(vcptr);
    m_allVideoWindow = new AllVideoWindow(vcptr, m_videoPreCtl->GetDevTree());

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(m_videoPreCtl, 3);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_allVideoWindow, 16);
    setLayout(m_mainLayout);
}

void VideoSurveil::SaveCus()
{
    m_allVideoWindow->SaveCus();
}
