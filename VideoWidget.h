#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>

class VideoWindow;
class QLabel;
class flowPanel;
class configFlowPanel;
class videoingLbl;
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(VideoWindow* vw, QWidget *parent = Q_NULLPTR, bool isReplay = false);

    void SetbPaint(bool paint);

    void SetVideoWin(VideoWindow* vw);

    void SetPanelVisible(bool v);

    HWND Gethwnd();

    VideoWindow* GetVW();

public slots:
    void btnClickedSlot();

protected:
    void paintEvent(QPaintEvent *event);

    virtual void resizeEvent(QResizeEvent* event);

    //鼠标进入事件
    virtual void enterEvent(QEvent* event);

    //鼠标离开事件
    virtual void leaveEvent(QEvent* event);

private:
    VideoWindow* m_vw;

    bool m_paintFrame = false;

    bool m_videoing = false;

    bool m_configPanelVisible = false;

    flowPanel* m_flowPanel;

    configFlowPanel* m_configPanel;

    videoingLbl* m_videoingLbl;
};

#endif // VIDEOWIDGET_H
