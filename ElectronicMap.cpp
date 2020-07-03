#include "ElectronicMap.h"
#include "ElectronicMapCtl.h"
#include "nodeinfobox.h"
#include "DeviceTree.h"
#include "VideoWidget.h"
#include "VideoWindow.h"
#include <QVBoxLayout>
#include <QWebEngineView>

ElectronicMap::ElectronicMap(int x, int y, QWidget *parent): QWidget(parent)
{
    IniUi(x, y);
}

void ElectronicMap::IniUi(int x, int y)
{

    m_view=new QWebEngineView(this);
    QString filePath = "D:/IntelliManagePlatform/baidumap2.html";
    m_view->page()->load(filePath);
    m_view->setFixedSize(x,y);

    ElectronicMapCtl* electronicMapCtl = new ElectronicMapCtl(this);
    electronicMapCtl->setFixedHeight(500);

    nodeInfoBox* nodeBox = new nodeInfoBox(this);

    nodeBox->move(682,306);
    nodeBox->hide();

    auto func = [nodeBox](TREE_ITEM_INFO* item){
        nodeBox->setInfo(item);
        nodeBox->GetRealTimeVideoWidget()->GetVW()->Close();
        nodeBox->GetRealTimeVideoWidget()->SetPanelVisible(false);
        nodeBox->GetRealTimeVideoWidget()->hide();
        nodeBox->show();
    };

    electronicMapCtl->GetDevTree()->SetFn(func);
/*
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QAxWidget* myWidget = new QAxWidget(this);
    mainLayout->addWidget(myWidget);
    myWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));//注册组件UUID
    myWidget->setFocusPolicy(Qt::StrongFocus);       //设置控件接收键盘焦点的方式：鼠标单击、Tab键
    myWidget->setProperty("DisplayAlerts",false);    //不显示任何警告信息。
    myWidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    QString webStr = QString("D:\\IntelliManagePlatform\\baidumap2.html");
    myWidget->dynamicCall("Navigate(const QString&)",webStr);
*/
}
