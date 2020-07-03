#include "ReplayTheaterCtl.h"
#include "DeviceTree.h"
#include "calendarSelect.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>

ReplayTheaterCtl::ReplayTheaterCtl(std::shared_ptr<VideoControl> vcptr) :  m_devTree(new DeviceTree(this,vcptr)),
    m_calendarSlt(new calendarSelectTime(this))
{
    InitUi();
}

DeviceTree *ReplayTheaterCtl::GetDevTree()
{
    return m_devTree;
}

void ReplayTheaterCtl::InitUi()
{
    setStyleSheet("background-color: #FFFFFFFF");
    m_mainLytWgt = new QWidget;
    m_mainLytWgt->setStyleSheet("background-color: #FFEBEFF7");
    QVBoxLayout* mainLyt = new QVBoxLayout(m_mainLytWgt);
    mainLyt->setContentsMargins(0, 0, 0, 0);
    QFont Font("MicrosoftYaHei", 16, QFont::Normal);
    Font.setPixelSize(16);

    m_btnBar = new QWidget;
    m_btnBar->setStyleSheet("background-color: #FFFFFF");
    QSizePolicy sizePolicy = this->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    m_btnBar->setSizePolicy(sizePolicy);
    QHBoxLayout* btnLyt = new QHBoxLayout(m_btnBar);
    btnLyt->setContentsMargins(0, 0, 0, 0);
    btnLyt->setSpacing(0);
    QPushButton* devBtn = new QPushButton("设备列表");
    SetBtnAttr(devBtn, Font);
    QPushButton* collectBtn = new QPushButton("收藏列表");
    SetBtnAttr(collectBtn, Font);
    QPushButton* hisBtn = new QPushButton("历史记录");
    SetBtnAttr(hisBtn, Font);
    connect(devBtn, &QPushButton::clicked, this, [=](){
        devBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        collectBtn->setStyleSheet("border:none;color:#777777;");
        hisBtn->setStyleSheet("border:none;color:#777777;");
    });
    connect(collectBtn, &QPushButton::clicked, this, [=](){
        collectBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        devBtn->setStyleSheet("border:none;color:#777777;");
        hisBtn->setStyleSheet("border:none;color:#777777;");
    });
    connect(hisBtn, &QPushButton::clicked, this, [=](){
        hisBtn->setStyleSheet("border:none;color:#FF1D4ABD;");
        devBtn->setStyleSheet("border:none;color:#777777;");
        collectBtn->setStyleSheet("border:none;color:#777777;");
    });

    btnLyt->addWidget(devBtn);
    btnLyt->addWidget(collectBtn);
    btnLyt->addWidget(hisBtn);

    m_deviceList = new QWidget;
    m_deviceList->setStyleSheet("background-color: #FFFFFF");
    QVBoxLayout* devLyt = new QVBoxLayout(m_deviceList);
    QLineEdit* searchEdit = new QLineEdit;
    QAction* searchAct = new QAction(searchEdit);
    searchAct->setIcon(QIcon(QPixmap(":/img/sousuo.png")));
    searchEdit->addAction(searchAct, QLineEdit::TrailingPosition);
    searchEdit->setPlaceholderText(tr("Please enter device name"));
    devLyt->addWidget(searchEdit, 32);
    devLyt->addWidget(m_devTree, 582);
    m_devTree->setDragDropMode(QAbstractItemView::DragOnly);

    QLabel* timeSltLbl = new QLabel("时间选择");
    timeSltLbl->setFont(Font);
    timeSltLbl->setStyleSheet("color:#333333;background-color:#FFFFFF");

    mainLyt->setSpacing(0);
    mainLyt->addWidget(m_btnBar,47);
    mainLyt->addSpacing(1);
    mainLyt->addWidget(m_deviceList,594);
    mainLyt->addSpacing(2);
    mainLyt->addWidget(timeSltLbl,49);
    mainLyt->addWidget(m_calendarSlt,333);
    m_calendarSlt->setStyleSheet("background-color: #FFFFFF");

    QHBoxLayout* hLyt = new QHBoxLayout(this);
    hLyt->setContentsMargins(0, 0, 0, 0);
    hLyt->setSpacing(0);
    QPushButton* backBtn = new QPushButton;
    SetPicBtnAttr(backBtn, ":/img/hideCtl.png");
    QSizePolicy btnsizePolicy = backBtn->sizePolicy();
    btnsizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    backBtn->setSizePolicy(btnsizePolicy);
    connect(backBtn,&QPushButton::clicked,this, [this,backBtn](){
        if(!m_mainLytWgt->isVisible())
        {
            setMinimumWidth(340);
            m_mainLytWgt->show();
            SetPicBtnAttr(backBtn, ":/img/hideCtl.png");
        }
        else
        {
            setMinimumWidth(0);
            QSizePolicy sizePolicy = this->sizePolicy();
            sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
            setSizePolicy(sizePolicy);
            m_mainLytWgt->hide();
            SetPicBtnAttr(backBtn, ":/img/showCtl.png");
        }
    });
    hLyt->addWidget(m_mainLytWgt,320);
    hLyt->addWidget(backBtn, 20);
    setMinimumWidth(340);
}

void ReplayTheaterCtl::SetBtnAttr(QPushButton *btn, const QFont &font)
{
    btn->setFont(font);
    btn->setStyleSheet("border:none;color:#777777;");
}

void ReplayTheaterCtl::SetPicBtnAttr(QPushButton *btn, const QString &picPath)
{
    btn->setStyleSheet("border:none;");
    btn->setIcon(QIcon(picPath));
}
