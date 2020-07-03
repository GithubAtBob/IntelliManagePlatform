#ifndef VIDEOPRECTL_H
#define VIDEOPRECTL_H

#include <QWidget>

enum class DirectionEnum
{
    TOP = 0,
    LEFTTOP = 1,
    LEFT = 2,
    LEFTBTM = 3,
    BTM = 4,
    RIGHTBTM = 5,
    RIGHT = 6,
    RIGHTTOP = 7,
    NARROW = 8,
    ENLARGE = 9
};

class QVBoxLayout;
class QPushButton;
class DeviceTree;
class VideoControl;
class VideoPreCtl : public QWidget
{
    Q_OBJECT
public:
    VideoPreCtl(std::shared_ptr<VideoControl> vcptr);

    DeviceTree* GetDevTree();

private:
    //初始化界面
    void InitUi();

    //设置按钮属性
    void SetBtnAttr(QPushButton* btn, const QFont& font);

    //设置图片按钮属性
    void SetPicBtnAttr(QPushButton* btn, const QString& picPath);

    void SetPicBtnAttr(QPushButton* btn, const QString& picPath, const QString& hoveredPath, const QString& tipStr);

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

    //云台控制标题栏
    QWidget* m_pantiltTitle;

/********云台控制********/
    QWidget* m_pantiltControl;

    bool m_multifuncExpand = false;

    bool m_pressed = false;
/********************/

private slots:
    void ButtonJudgeSlot(int idx);

    void DirectionPressedSlot(int buttonId);

    void DirectionReleasedSlot(int buttonId);

    void sliderValueChangeSlot(int value);
};

#endif // VIDEOPRECTL_H
