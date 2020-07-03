#ifndef SLIDINGVIDEOPLAYTIMELINE_H
#define SLIDINGVIDEOPLAYTIMELINE_H

#include <QWidget>
#include "videolisttimeline.h"
#include "timeslider.h"

class slidingVideoPlayTimeLine : public QWidget
{
    Q_OBJECT
public:
    explicit slidingVideoPlayTimeLine(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
signals:

private:
    VideoListTimeLine* timeLine;
    timeSlider* slider;
};

#endif // SLIDINGVIDEOPLAYTIMELINE_H
