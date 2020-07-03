#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include <QSlider>

class timeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit timeSlider(QWidget *parent = nullptr);
protected:

    void mousePressEvent(QMouseEvent *ev);


signals:

};

#endif // TIMESLIDER_H
