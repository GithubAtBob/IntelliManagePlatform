#ifndef _VIDEOLISTTIMELINE_
#define _VIDEOLISTTIMELINE_

#include <QWidget>
#include <QDate>

class VideoListTimeLine : public QWidget
{
    Q_OBJECT
public:
    VideoListTimeLine(QWidget *parent = 0,int height = 35);
    ~VideoListTimeLine();

    void setVideoListTime(QList<QPair<QTime, QTime> > recordList);
    void setScaleFactor(int scaleFactor);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);
signals:
    void mouseDoubleClickSignal(int sec);

private:
    int     timeLineWidth;
    int     timeLineHeight;
    int     scaleFactor = 1;        // 伸缩因子

    int     rightHiddenWidth = 0;       // 左右移动时间轴，右侧未显示的宽度
    int     rightHiddenMiniWidth = 0;   // 右侧未显示的最小宽度 0
    int     rightHiddenMaxWidth = 0;    // 右侧未显示的最大宽度 timeLineWidth - this->width()

    int     startPosX = 0;
    bool    mouseRightButtonClicked = false;
    QPoint  mousePressedPosition;

    bool    _30minutesRulerFlag = false;   //30分钟刻度尺
    bool    _15minutesRulerFlag = false;    // 15分钟刻度尺
    bool    _5minutesRulerFlag = false;     // 5分钟刻度尺
    bool    _1minuteRulerFlag = false;      // 1分钟刻度尺

    QList< QPair<QTime, QTime> >    videoList;
    QDate   curDate;

    QList<QRectF> convertTimeSegmentToRect();
    void paintTimeSegment();
    void paintTimeRuler();
    void setTimeRulerFlag();

    void paintOneHourRuler();
    void paintHalfHourRuler();
    void paint15minutesRuler();
    void paint5minutesRuler();
    void paint1minuteRuler();
    void paint12minutesRuler();
};

#endif
