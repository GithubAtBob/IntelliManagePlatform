#ifndef VIDEOSURVEIL_H
#define VIDEOSURVEIL_H

#include "AllVideoWindow.h"
#include <QHBoxLayout>
#include <QResizeEvent>

class VideoPreCtl;
class VideoSurveil : public QWidget
{
	Q_OBJECT

public:
	VideoSurveil(QWidget *parent = Q_NULLPTR);

    //保存自定义数据
    void SaveCus();
private:
	QHBoxLayout* m_mainLayout;

	AllVideoWindow* m_allVideoWindow;

    //左侧控制界面
    VideoPreCtl* m_videoPreCtl;
};

#endif
