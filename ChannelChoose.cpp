#include "ChannelChoose.h"
#include "AllVideoWindow.h"
#include "AddCustomizeChannel.h"
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

ChannelChoose::ChannelChoose(std::shared_ptr<AddCustomizeChannel> addCus, QWidget *parent) : QWidget(parent),
    m_prtPtr(static_cast<AllVideoWindow*>(parent)), m_addCus(addCus), m_btnGrp(new QButtonGroup)
{
    InitUi();
}

void ChannelChoose::AddCusBtn(QPushButton *btn)
{
    QFont btnfont("MicrosoftYaHei", 14, QFont::Normal);
    btnfont.setPixelSize(14);
    SetPicBtnAttr(btn,":/img/23.1.png");
    btn->setParent(this);
    btn->move(m_pos,236);
    connect(btn, &QPushButton::toggled, this, [this, btn](bool checked){
        if(checked)
            SetPicBtnAttr(btn, ":/img/23.1copy.png");
        else
            SetPicBtnAttr(btn, ":/img/23.1.png");
    });
    m_btnGrp->addButton(btn);
    QLabel* lbl = new QLabel(btn->objectName(),this);
    lbl->setFixedSize(22,12);
    lbl->move(m_pos+3,271);
    lbl->setFont(btnfont);
    lbl->setStyleSheet("color:#A1A1A1;");
    m_pos += 50;
    m_addCusBtn->move(m_pos,236);
}

void ChannelChoose::InitUi()
{
    m_addCus->hide();
    setWindowFlags(Qt::FramelessWindowHint| Qt::Popup);
    setStyleSheet("background-color: #FFFFFF");
    setFixedSize(500, 310);
    QLabel* bgLbl = new QLabel(this);
    bgLbl->setFixedSize(500, 310);
    bgLbl->setStyleSheet("background-color: #FFFFFF");

    QLabel* titleLbl = new QLabel("选择画面",this);
    titleLbl->move(20, 20);
    titleLbl->setFixedSize(67, 16);
    QFont titlefont("MicrosoftYaHei", 16, QFont::Normal);
    titlefont.setPixelSize(16);
    titleLbl->setFont(titlefont);
    titleLbl->setStyleSheet("color:#333333;");

    /*
    QPushButton* closeBtn = new QPushButton(this);
    SetPicBtnAttr(closeBtn, ":/img/guanbi2.png");
    closeBtn->move(468, 20);
    connect(closeBtn, &QPushButton::clicked, this, [this](){hide();});
    */

    QPushButton* oneoneBtn = new QPushButton(this);
    SetPicBtnAttr(oneoneBtn, ":/img/1.unsel.png");
    oneoneBtn->move(20, 64);
    connect(oneoneBtn, &QPushButton::toggled, this, [this, oneoneBtn](bool checked){
        if(checked)
            SetPicBtnAttr(oneoneBtn, ":/img/1.1.png");
        else
            SetPicBtnAttr(oneoneBtn, ":/img/1.unsel.png");
    });

    QPushButton* twotwoBtn = new QPushButton(this);
    SetPicBtnAttr(twotwoBtn, ":/img/22.1.png");
    twotwoBtn->move(70, 64);
    twotwoBtn->setChecked(true);
    connect(twotwoBtn, &QPushButton::toggled, this, [this, twotwoBtn](bool checked){
        if(checked)
            SetPicBtnAttr(twotwoBtn, ":/img/22.1.png");
        else
            SetPicBtnAttr(twotwoBtn, ":/img/22.unsel.png");
    });

    QPushButton* threethreeBtn = new QPushButton(this);
    SetPicBtnAttr(threethreeBtn, ":/img/33.unsel.png");
    threethreeBtn->move(120, 64);
    connect(threethreeBtn, &QPushButton::toggled, this, [this, threethreeBtn](bool checked){
        if(checked)
            SetPicBtnAttr(threethreeBtn, ":/img/33.1.png");
        else
            SetPicBtnAttr(threethreeBtn, ":/img/33.unsel.png");
    });

    QPushButton* fourfourBtn = new QPushButton(this);
    SetPicBtnAttr(fourfourBtn, ":/img/16.unsel.png");
    fourfourBtn->move(170, 64);
    connect(fourfourBtn, &QPushButton::toggled, this, [this, fourfourBtn](bool checked){
        if(checked)
            SetPicBtnAttr(fourfourBtn, ":/img/16.1.png");
        else
            SetPicBtnAttr(fourfourBtn, ":/img/16.unsel.png");
    });

    QPushButton* fivefiveBtn = new QPushButton(this);
    SetPicBtnAttr(fivefiveBtn, ":/img/25.unsel.png");
    fivefiveBtn->move(220, 64);
    connect(fivefiveBtn, &QPushButton::toggled, this, [this, fivefiveBtn](bool checked){
        if(checked)
            SetPicBtnAttr(fivefiveBtn, ":/img/25.1.png");
        else
            SetPicBtnAttr(fivefiveBtn, ":/img/25.unsel.png");
    });

    QFont btnfont("MicrosoftYaHei", 14, QFont::Normal);
    btnfont.setPixelSize(14);

    QLabel* oneoneLbl = new QLabel("1*1",this);
    oneoneLbl->setFixedSize(22,12);
    oneoneLbl->move(24,99);
    oneoneLbl->setFont(btnfont);
    oneoneLbl->setStyleSheet("color:#A1A1A1;");

    QLabel* twotwoLbl = new QLabel("2*2",this);
    twotwoLbl->setFixedSize(22,12);
    twotwoLbl->move(73,99);
    twotwoLbl->setFont(btnfont);
    twotwoLbl->setStyleSheet("color:#A1A1A1;");

    QLabel* threethreeLbl = new QLabel("3*3",this);
    threethreeLbl->setFixedSize(22,12);
    threethreeLbl->move(123,99);
    threethreeLbl->setFont(btnfont);
    threethreeLbl->setStyleSheet("color:#A1A1A1;");

    QLabel* fourfourLbl = new QLabel("4*4",this);
    fourfourLbl->setFixedSize(22,12);
    fourfourLbl->move(172,99);
    fourfourLbl->setFont(btnfont);
    fourfourLbl->setStyleSheet("color:#A1A1A1;");

    QLabel* fivefiveLbl = new QLabel("5*5",this);
    fivefiveLbl->setFixedSize(22,12);
    fivefiveLbl->move(224,99);
    fivefiveLbl->setFont(btnfont);
    fivefiveLbl->setStyleSheet("color:#A1A1A1;");

    QPushButton* onesevenBtn = new QPushButton(this);
    SetPicBtnAttr(onesevenBtn, ":/img/1+7.1.png");
    onesevenBtn->move(20, 152);
    connect(onesevenBtn, &QPushButton::toggled, this, [this, onesevenBtn](bool checked){
        if(checked)
            SetPicBtnAttr(onesevenBtn, ":/img/1+7.sel.png");
        else
            SetPicBtnAttr(onesevenBtn, ":/img/1+7.1.png");
    });

    QLabel* onesevenLbl = new QLabel("1+7",this);
    onesevenLbl->setFixedSize(22,12);
    onesevenLbl->move(24,187);
    onesevenLbl->setFont(btnfont);
    onesevenLbl->setStyleSheet("color:#A1A1A1;");

    m_addCusBtn = new QPushButton(this);
    SetPicBtnAttr(m_addCusBtn, ":/img/tianjiazidingyi.png");
    m_addCusBtn->move(20,236);
    connect(m_addCusBtn, &QPushButton::clicked, this, [this](){
        QPoint r(m_prtPtr->width(),m_prtPtr->height());
        QPoint gloPos = m_prtPtr->mapToGlobal(r);
        int w = gloPos.x();
        int h = gloPos.y();
        m_addCus->move(w-520,h-710);
        m_addCus->show();
        hide();
    });

    m_btnGrp->addButton(oneoneBtn,static_cast<int>(Video::VideoControlEnum::Full));
    m_btnGrp->addButton(twotwoBtn,static_cast<int>(Video::VideoControlEnum::Four));
    m_btnGrp->addButton(threethreeBtn,static_cast<int>(Video::VideoControlEnum::Nine));
    m_btnGrp->addButton(fourfourBtn,static_cast<int>(Video::VideoControlEnum::Sixteen));
    m_btnGrp->addButton(fivefiveBtn,static_cast<int>(Video::VideoControlEnum::Twentyfive));
    m_btnGrp->addButton(onesevenBtn,static_cast<int>(Video::VideoControlEnum::Eight));
    connect(m_btnGrp, SIGNAL(buttonClicked(int)), m_prtPtr, SLOT(ButtonJudge(int)));
}

void ChannelChoose::SetPicBtnAttr(QPushButton *btn, const QString &picPath)
{
    btn->setStyleSheet("border:none;");
    btn->resize(QPixmap(picPath).size());
    btn->setIcon(QIcon(picPath));
    btn->setIconSize(btn->size());
    btn->setCheckable(true);
}
