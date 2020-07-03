#ifndef VIDEOFLOWPANEL_H
#define VIDEOFLOWPANEL_H

#include "flowPanel.h"

class VideoFlowPanel : public flowPanel
{
    Q_OBJECT
public:
    VideoFlowPanel(QWidget *parent = Q_NULLPTR);

    //初始化浮窗
    void InitFlowPanel();
};

#endif // VIDEOFLOWPANEL_H
