#ifndef VIDEOPLAYTIMECTRL_H
#define VIDEOPLAYTIMECTRL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCalendarWidget>

class calendarSelectTime : public QWidget
{
    Q_OBJECT
public:
    explicit calendarSelectTime(QWidget *parent = nullptr);
    void init();
    void init1();
    void setDataLabelTimeText(int year, int month);

signals:

private slots:
    void onbtnClicked();

private:
    QCalendarWidget* dateWd;
    QLabel* dateLab;
    QPushButton* upDateBtn;
    QPushButton* backwardDateBtn;
};

#endif // VIDEOPLAYTIMECTRL_H
