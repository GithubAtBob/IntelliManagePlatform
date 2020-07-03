#ifndef VWIDGET_H
#define VWIDGET_H

#include <QWidget>
#include <QDateTime>

class VideoControl;
class VideoWidget;
class vWidget : public QWidget
{
    Q_OBJECT
public:
    vWidget(std::shared_ptr<VideoControl> vcptr, QWidget *parent = Q_NULLPTR);

protected:
    //键盘事件
    virtual void keyPressEvent(QKeyEvent *event);

    //鼠标移动事件
    virtual void mouseMoveEvent(QMouseEvent* event);

    QDateTime m_lastTime = QDateTime::currentDateTime();

    bool m_cursorHide = false;

private:
    std::shared_ptr<VideoControl> m_vcptr;

    VideoWidget* m_currentWidget = nullptr;

private slots:
    void checkCursorSlot();
};

#endif // VWIDGET_H
