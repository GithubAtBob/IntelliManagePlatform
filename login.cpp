#include "login.h"
#include "index.h"
#include "ToolInstance.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QMouseEvent>
#include <QLineEdit>
#include <QAction>
#include <QIcon>
#include <QCheckBox>
#include <QPixmap>
#include <QSettings>

login::login(QWidget *parent)
    : QMainWindow(parent)
{

    InitUi();
}

void login::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

void login::mouseMoveEvent(QMouseEvent * event)
{
    if (m_bPressed)
        move(event->pos() - m_point + pos());
}

void login::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

bool login::eventFilter(QObject * obj, QEvent * e)
{
    if (obj == m_unEdit && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = (QMouseEvent*)e;
        if (me->button() == Qt::LeftButton)
        {
            m_pwdWidget->hide();
            m_unWidget->show();
        }
    }
    else if (obj == m_pwdEdit && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = (QMouseEvent*)e;
        if (me->button() == Qt::LeftButton)
        {
            m_unWidget->hide();
            m_pwdWidget->show();
        }
    }
    return QMainWindow::eventFilter(obj, e);
}

void login::InitUi()
{
    setFixedSize(1000, 620);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: #EBEFF8");
    QLabel* bgLbl = new QLabel(this);
    bgLbl->setFixedSize(1000, 620);
    bgLbl->setPixmap(QPixmap(":/img/background.png"));


    QFrame* logFrame = new QFrame(this);
    logFrame->move(596, 120);
    logFrame->setFixedSize(340, 420);
    logFrame->setFrameShape(QFrame::Panel);
    logFrame->setFrameShadow(QFrame::Raised);
    logFrame->setMidLineWidth(5);
    logFrame->setStyleSheet("background-color: white;");

    m_unWidget = new QWidget(logFrame);
    m_unWidget->move(18, 120);
    m_unWidget->setFixedSize(304, 44);
    m_unWidget->setStyleSheet("border:1px solid #FF6E35;");
    m_unWidget->hide();

    m_pwdWidget = new QWidget(logFrame);
    m_pwdWidget->move(18, 190);
    m_pwdWidget->setFixedSize(304, 44);
    m_pwdWidget->setStyleSheet("border:1px solid #FF6E35;");
    m_pwdWidget->hide();

    QLabel* titleLbl = new QLabel(tr("Intelligent management platform"),logFrame);
    titleLbl->move(93, 78);
    titleLbl->setFixedSize(154, 25);
    QFont titlefont("MicrosoftYaHei", 12, QFont::Normal);
    titlefont.setPixelSize(24);
    titleLbl->setFont(titlefont);
    titleLbl->setStyleSheet("color:#1D4ABD;");

    QFont editfont("MicrosoftYaHei", 7, QFont::Normal);
    editfont.setPixelSize(14);

    ToolInstance* i = ToolInstance::GetInstance();
    m_un = i->ReadIni("D:\\IntelliManagePlatform\\config.ini", "/user/username");
    m_pwd = i->ReadIni("D:\\IntelliManagePlatform\\config.ini", "/user/password");
    m_unEdit = new QLineEdit(logFrame);
    QAction* unAct = new QAction(m_unEdit);
    unAct->setIcon(QIcon(QPixmap(":/img/username.png")));
    m_unEdit->addAction(unAct, QLineEdit::LeadingPosition);
    m_unEdit->setPlaceholderText(tr("Please enter username"));
    if (m_un != "")
    {
        m_unEdit->setText(m_un);
    }
    m_unEdit->move(20, 122);
    m_unEdit->setFixedSize(300, 40);
    m_unEdit->installEventFilter(this);
    m_unEdit->setFont(editfont);
    connect(m_unEdit, &QLineEdit::textChanged, this, [this](const QString& text) {m_un = text; });
    m_pwdEdit = new QLineEdit(logFrame);
    QAction* pwdAct = new QAction(m_pwdEdit);
    pwdAct->setIcon(QIcon(QPixmap(":/img/password.png").scaled(20, 23)));
    m_pwdEdit->addAction(pwdAct, QLineEdit::LeadingPosition);
    m_pwdEdit->setPlaceholderText(tr("Please enter password"));
    if (m_pwd != "")
    {
        m_pwdEdit->setText(m_pwd);
    }
    m_pwdEdit->move(20, 192);
    m_pwdEdit->setFixedSize(300, 40);
    m_pwdEdit->installEventFilter(this);
    m_pwdEdit->setFont(editfont);
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    connect(m_pwdEdit, &QLineEdit::textChanged, this, [this](const QString& text) {m_pwd = text; });

    QCheckBox* isRmbrPwdCheckBox = new QCheckBox(logFrame);
    isRmbrPwdCheckBox->move(20, 252);
    isRmbrPwdCheckBox->setFixedSize(20, 20);
    connect(isRmbrPwdCheckBox, &QCheckBox::clicked, this, [this]() {m_savePwd = !m_savePwd; });
    QLabel* rmbrPwdLbl = new QLabel(tr("Remember Password"), logFrame);
    rmbrPwdLbl->move(50, 253);
    rmbrPwdLbl->setFixedSize(68, 16);
    QFont rmbrpwdfont("MicrosoftYaHei", 8, QFont::Normal);
    rmbrpwdfont.setPixelSize(16);
    rmbrPwdLbl->setFont(rmbrpwdfont);
    rmbrPwdLbl->setStyleSheet("color:#333333;");

    QPushButton* logBtn = new QPushButton(tr("login"), logFrame);
    logBtn->move(40, 298);
    logBtn->setFixedSize(260, 40);
    logBtn->setStyleSheet("border-radius:20px;color:#FFFFFF;background-color:#1D4ABD");
    QFont logfont("MicrosoftYaHei", 10, QFont::Normal);
    logfont.setPixelSize(20);
    logBtn->setFont(logfont);
    connect(logBtn, &QPushButton::clicked, this, &login::LoginSlot);

    QPushButton* setBtn = new QPushButton(logFrame);
    setBtn->move(291, 370);
    setBtn->setFixedSize(30, 30);
    setBtn->setIcon(QIcon(":/img/setting.png"));
    setBtn->setStyleSheet("border:none;");
    connect(setBtn, &QPushButton::clicked, this, [this]() {setFixedSize(1300, 620); });


    QPushButton* closeBtn = new QPushButton(bgLbl);
    closeBtn->move(962, 17);
    closeBtn->setFixedSize(18, 18);
    closeBtn->setIcon(QIcon(":/img/close.png"));
    closeBtn->setStyleSheet("border:none;");
    connect(closeBtn, &QPushButton::clicked, this, [this]() {close(); });


    QLabel* ipLbl = new QLabel(tr("Sevice IP"), this);
    ipLbl->move(1019, 190);
    ipLbl->setFixedSize(57, 19);
    QFont ipfont("MicrosoftYaHei", 9, QFont::Normal);
    ipfont.setPixelSize(18);
    ipLbl->setFont(ipfont);
    ipLbl->setStyleSheet("color:#333333;");
    QLineEdit* ipEdit = new QLineEdit(this);
    ipEdit->move(1020, 222);
    ipEdit->setFixedSize(260, 38);
    ipEdit->setPlaceholderText(tr("Chinese"));
    ipEdit->setFont(editfont);
    QLabel* portLbl = new QLabel(tr("Port"), this);
    portLbl->move(1019, 279);
    portLbl->setFixedSize(36, 19);
    QFont portfont("MicrosoftYaHei", 9, QFont::Normal);
    portfont.setPixelSize(18);
    portLbl->setFont(portfont);
    portLbl->setStyleSheet("color:#333333;");
    QLineEdit* portEdit = new QLineEdit(this);
    portEdit->move(1020, 311);
    portEdit->setFixedSize(260, 38);
    portEdit->setPlaceholderText("9900");
    portEdit->setFont(editfont);
    QPushButton* backBtn = new QPushButton(this);
    backBtn->move(1046, 392);
    backBtn->setFixedSize(210, 38);
    backBtn->setStyleSheet("border-radius:19px;background-color:#1D4ABD");
    backBtn->setIcon(QIcon(":/img/back.png"));
    connect(backBtn, &QPushButton::clicked, this, [this]() {setFixedSize(1000, 620); });
}

void login::LoginSlot()
{
    ToolInstance* i = ToolInstance::GetInstance();
    if (m_savePwd)
    {
        i->WriteIni("D:\\IntelliManagePlatform\\config.ini", "/user/username", m_un);
        i->WriteIni("D:\\IntelliManagePlatform\\config.ini", "/user/password", m_pwd);
    }
    else
    {
        i->WriteIni("D:\\IntelliManagePlatform\\config.ini", "/user/username", "");
        i->WriteIni("D:\\IntelliManagePlatform\\config.ini", "/user/password", "");
    }
    //异步处理验证消息
    bool bRet = false;
    QString stAccount,stPsw,stIp;
    stAccount = "zjxc@kfq.jhhy.zj.ge";
    stPsw = "ef73781effc5774100f87fe2f437a435";
    stIp= "122.226.82.21:6666";
    m_v3VideoPlayInterface.InitDll();
    bRet = m_v3VideoPlayInterface.Login(stAccount.toStdString().c_str(),stPsw.toStdString().c_str(),stIp.toStdString().c_str());
    if(bRet)
    {
        hide();
        m_v3VideoPlayInterface.GetDeviceList();
        m_index = new index;
        m_index->show();
    }
}
