#ifndef REPLAYTHEATER_H
#define REPLAYTHEATER_H

#include <QWidget>

class QHBoxLayout;
class AllReplayWindow;
class ReplayTheaterCtl;
class ReplayTheater : public QWidget
{
    Q_OBJECT

public:
    ReplayTheater(QWidget *parent = Q_NULLPTR);

private:
    QHBoxLayout* m_mainLayout;

    AllReplayWindow* m_allReplayWindow;

    //左侧控制界面
    ReplayTheaterCtl* m_replayTheaterCtl;
};

#endif // REPLAYTHEATER_H
