#include "calendarSelect.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include "qcustomcalendarwidget.h"
#include "videolisttimeline.h"
#include "slidingvideoplaytimeline.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

calendarSelectTime::calendarSelectTime(QWidget *parent) : QWidget(parent)
{
    init1();
}

void calendarSelectTime::init1()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(320,220);

    QWidget* wd = new QWidget(this);
    wd->setFixedSize(width(),height());
    wd->setStyleSheet("background:rgba(5,15,43,1);rgba(157,157,157,0.49);"
                      "opacity:0.8;border-radius:4px;");

    QLabel* calBackground = new QLabel(wd);
    QPixmap pixmap(":/img/riqibeijing.png");
    calBackground->setFixedSize(pixmap.size());
    calBackground->setPixmap(pixmap);
    calBackground->move(83,0);

    QPushButton* calendarBtn = new QPushButton(calBackground);
    QPixmap pic(":/img/rili.png");
    calendarBtn->setFixedSize(pic.size());
    calendarBtn->setIcon(pic);
    calendarBtn->move(28,12);

    dateLab = new QLabel(calBackground);
    dateLab->setStyleSheet("width:73px;height:14px;font-size:16px;"
                           "font-family:Microsoft YaHei UI;font-weight:bold;color:rgba(29,74,189,1);line-height:28px;");
    dateLab->move(54,15);

    upDateBtn = new QPushButton(QIcon(":/img/xiangqian.png"),NULL,wd);
    upDateBtn->setFixedSize(QPixmap(":/img/xiangqian.png").size());
    upDateBtn->setIcon(QIcon(":/img/xiangqian.png"));
    upDateBtn->move(16,0);
    upDateBtn->setStyleSheet("border:none");

    backwardDateBtn = new QPushButton(QIcon(":/img/xianghou.png"),NULL,wd);
    backwardDateBtn->setFixedSize(QPixmap(":/img/xianghou.png").size());
    backwardDateBtn->setIcon(QIcon(":/img/xianghou.png"));
    backwardDateBtn->move(268,0);
    backwardDateBtn->setStyleSheet("border:none");

    dateWd = new QCalendarWidget(wd);
    dateWd->move(10,60);
    dateWd->setFixedSize(300,160);
    setLocale(QLocale(QLocale::Chinese));
    dateWd->setStyleSheet("font{font-size:14px;font-family:Microsoft YaHei;font-weight:400;color:rgba(171,171,171,1);}");
//    dateWd->setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
//    dateWd->setAttribute(Qt::WA_TranslucentBackground, true);
//    dateWd->setWindowOpacity(0.5);

//    dateWd->setAutoFillBackground(false);
//    dateWd->setAttribute(Qt::WA_TranslucentBackground, true);

    dateWd->setNavigationBarVisible(false);
    dateWd->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    dateWd->setHorizontalHeaderFormat(QCalendarWidget::NoHorizontalHeader);

    QLabel* SundayLab = new QLabel(wd);
    SundayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    SundayLab->setText("周日");
    SundayLab->move(21,40);

    QLabel* MondayLab = new QLabel(wd);
    MondayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    MondayLab->setText("周一");
    MondayLab->move(61,40);

    QLabel* TuesdayLab = new QLabel(wd);
    TuesdayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    TuesdayLab->setText("周二");
    TuesdayLab->move(103,40);

    QLabel* WednesdayLab = new QLabel(wd);
    WednesdayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    WednesdayLab->setText("周三");
    WednesdayLab->move(145,40);

    QLabel* ThursdayLab = new QLabel(wd);
    ThursdayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    ThursdayLab->setText("周四");
    ThursdayLab->move(187,40);

    QLabel* FridayLab = new QLabel(wd);
    FridayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    FridayLab->setText("周五");
    FridayLab->move(229,40);

    QLabel* SaturdayLab = new QLabel(wd);
    SaturdayLab->setStyleSheet("font-size:14px;font-family:Microsoft YaHei;font-weight:400;"
                          "color:rgba(161,161,161,1);line-height:28px;");
    SaturdayLab->setText("周六");
    SaturdayLab->move(272,40);



    connect(dateWd, &QCalendarWidget::currentPageChanged, [this](int year, int month){
        setDataLabelTimeText(year, month);
    });

    connect(upDateBtn,   SIGNAL(clicked()),  this, SLOT(onbtnClicked()));
    connect(backwardDateBtn,  SIGNAL(clicked()),  this, SLOT(onbtnClicked()));

    setDataLabelTimeText(dateWd->selectedDate().year(), dateWd->selectedDate().month());

}

void calendarSelectTime::init()
{
    QVBoxLayout* vLyt = new QVBoxLayout;
    setLayout(vLyt);

    QWidget* topwgt = new QWidget;
    QHBoxLayout* hLyt = new QHBoxLayout(topwgt);

   QPushButton* btnRili = new QPushButton;
   QPixmap pic(":/img/rili.png");
   btnRili->setFixedSize(pic.size());
   btnRili->setFocusPolicy(Qt::NoFocus);
   btnRili->setIcon(pic);
   btnRili->setStyleSheet("border:none");
   hLyt->addWidget(btnRili);

   QLabel* dateLab = new QLabel;
   dateLab->setStyleSheet("width:73px;height:14px;font-size:16px;"
                          "font-family:Microsoft YaHei UI;font-weight:bold;color:rgba(255,255,255,1);line-height:28px;");
}

void calendarSelectTime::setDataLabelTimeText(int year, int month)
{
    QString monthStr = (month<10)?("0"+QString::number(month)):QString::number(month);
    dateLab->setText(QString::number(year)+"-"+monthStr);
}

void calendarSelectTime::onbtnClicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    if (senderBtn == upDateBtn)
    {
        dateWd->showPreviousMonth();
    }
    else if (senderBtn == backwardDateBtn)
    {
        dateWd->showNextMonth();
    }
}
