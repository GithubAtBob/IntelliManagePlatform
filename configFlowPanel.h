#ifndef CONFIGFLOWPANEL_H
#define CONFIGFLOWPANEL_H

#include <QWidget>
class VideoWidget;
class configFlowPanel : public QWidget
{
    Q_OBJECT
public:
    configFlowPanel(QWidget *parent = Q_NULLPTR);

    void paintEvent(QPaintEvent *event);

private slots:
    void sliderValueChangeSlot(int value);

private:
    VideoWidget* m_prt;

    //初始化声音控制浮窗
    void InitConfigPanel();
};


#endif // CONFIGFLOWPANEL_H
