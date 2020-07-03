#ifndef DEVICETREE_H
#define DEVICETREE_H

#include <QTreeView>

class QLabel;
class QStandardItem;
class VideoControl;
struct TREE_ITEM_INFO;
class DeviceTree : public QTreeView
{
    Q_OBJECT
public:
    explicit DeviceTree(QWidget *parent = nullptr, std::shared_ptr<VideoControl> vcptr = nullptr);

    //获取选中条目
    QStandardItem* GetCurrItem();

    void SetFn(std::function<void(TREE_ITEM_INFO*)> f);

protected:
    //void mouseMoveEvent(QMouseEvent* event);
private:
    //初始化
    void Init();

    //增加通道
    void IncreaseChannel(int num);

    //绘制矩形
    QLabel* m_rect;

    std::shared_ptr<VideoControl> m_vcptr;

    QStandardItem* m_lastChecked = nullptr;

    std::function<void(TREE_ITEM_INFO*)> m_func;
private slots:
    //鼠标右键弹出菜单槽函数
    //void ShowContextMenuSlot(const QPoint& pos);

    //鼠标单击槽函数
    //void ClickSlot(const QModelIndex& index);

    //鼠标双击槽函数
    void DoubleClickSlot(const QModelIndex& index);

    void CurrentChangedSlot(const QModelIndex &current, const QModelIndex &previous);
};

#endif // DEVICETREE_H
