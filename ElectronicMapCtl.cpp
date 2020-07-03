#include "ElectronicMapCtl.h"
#include "DeviceTree.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>

ElectronicMapCtl::ElectronicMapCtl(QWidget *parent) : QWidget(parent),
    m_devTree(new DeviceTree)
{
    InitUi();
}

DeviceTree *ElectronicMapCtl::GetDevTree()
{
    return m_devTree;
}

void ElectronicMapCtl::InitUi()
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

    mainLyt->addWidget(m_btnBar,67);
    mainLyt->addSpacing(1);
    mainLyt->addWidget(m_deviceList,613);
    mainLyt->addSpacing(2);
//    mainLyt->addWidget(m_pantiltTitle,40);
//    mainLyt->addSpacing(1);
//    mainLyt->addWidget(m_pantiltControl, 300);

    QHBoxLayout* hLyt = new QHBoxLayout(this);
    hLyt->setContentsMargins(0, 0, 0, 0);
    hLyt->setSpacing(0);
    hLyt->addWidget(m_mainLytWgt,300);

    setMinimumWidth(300);
}

void ElectronicMapCtl::SetBtnAttr(QPushButton *btn, const QFont &font)
{
    btn->setFont(font);
    btn->setStyleSheet("border:none;color:#777777;");
}
