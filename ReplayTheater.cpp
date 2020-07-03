#include "ReplayTheater.h"
#include "VideoControl.h"
#include "AllReplayWindow.h"
#include "ReplayTheaterCtl.h"
#include <QHBoxLayout>

ReplayTheater::ReplayTheater(QWidget *parent)
    : QWidget(parent), m_mainLayout(new QHBoxLayout(this))
{
    auto vcptr = std::make_shared<VideoControl>();
    m_replayTheaterCtl = new ReplayTheaterCtl(vcptr);
    m_allReplayWindow = new AllReplayWindow(vcptr, m_replayTheaterCtl->GetDevTree());

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(m_replayTheaterCtl, 3);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_allReplayWindow, 16);
    setLayout(m_mainLayout);
}
