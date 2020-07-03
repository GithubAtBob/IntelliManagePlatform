#ifndef REPLAYTHEATERCTL_H
#define REPLAYTHEATERCTL_H

#include <QWidget>

class VideoControl;
class DeviceTree;
class QPushButton;
class calendarSelectTime;
class ReplayTheaterCtl : public QWidget
{
    Q_OBJECT
public:
    ReplayTheaterCtl(std::shared_ptr<VideoControl> vcptr);

    DeviceTree* GetDevTree();

private:
    //初始化界面
    void InitUi();

    //设置按钮属性
    void SetBtnAttr(QPushButton* btn, const QFont& font);

    void SetPicBtnAttr(QPushButton* btn, const QString& picPath);

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

    //时间选择
    calendarSelectTime* m_calendarSlt;

private slots:

};

#endif // REPLAYTHEATERCTL_H
