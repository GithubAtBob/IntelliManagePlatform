#ifndef FLOWPANEL_H
#define FLOWPANEL_H

#include <QWidget>
class VideoWidget;
class flowPanel : public QWidget
{
    Q_OBJECT
public:
    flowPanel(QWidget *parent = Q_NULLPTR);

    virtual ~flowPanel();

    //void paintEvent(QPaintEvent *event);

protected:
    VideoWidget* m_prt;

};

#endif // FLOWPANEL_H
