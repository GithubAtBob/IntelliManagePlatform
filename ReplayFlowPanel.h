#ifndef REPLAYFLOWPANEL_H
#define REPLAYFLOWPANEL_H

#include "flowPanel.h"

class QLabel;
class ReplayFlowPanel : public flowPanel
{
    Q_OBJECT
public:
    ReplayFlowPanel(QWidget *parent = Q_NULLPTR);

    //初始化浮窗
    void InitFlowPanel();

    //调整回放速度
    void AdjustReplaySpeed(bool fast);

private:
    double m_speed = 1.0;

    QLabel* m_speedLbl;
};


#endif // REPLAYFLOWPANEL_H
