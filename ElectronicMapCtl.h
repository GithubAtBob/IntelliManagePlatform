#ifndef ELECTRONICMAPCTL_H
#define ELECTRONICMAPCTL_H

#include <QWidget>

class DeviceTree;
class QPushButton;
class ElectronicMapCtl : public QWidget
{
    Q_OBJECT
public:
    ElectronicMapCtl(QWidget *parent = nullptr);

    DeviceTree* GetDevTree();

private:
    //初始化界面
    void InitUi();

    //设置按钮属性
    void SetBtnAttr(QPushButton* btn, const QFont& font);

    //主布局
    QWidget* m_mainLytWgt;

    //设备控制
    QWidget* m_devControl;

    //按钮栏
    QWidget* m_btnBar;

/********设备列表********/
    QWidget* m_deviceList;

    //设备树
    DeviceTree* m_devTree;
/********************/

    //收藏列表
    QWidget* m_collectionList;

    //历史记录
    QWidget* m_hisRecord;


private slots:

};

#endif // ELECTRONICMAPCTL_H
