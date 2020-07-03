#ifndef ALLVIDEOWINDOW_H
#define ALLVIDEOWINDOW_H

#include "VideoControl.h"
#include "AddCustomizeChannel.h"
#include "EditCustomizeChannel.h"
#include <QButtonGroup>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <memory>
#include <set>
#include <map>

class ChannelChoose;
class vWidget;
class DeviceTree;
class AllVideoWindow : public QWidget
{
	Q_OBJECT

public:
    AllVideoWindow(std::shared_ptr<VideoControl> vc, DeviceTree* devtree);

	//切换视频显示样式
	void resetShow(Video::VideoControlEnum i);

    //保存自定义数据
    void SaveCus();

protected:
	//鼠标点击事件
	virtual void mousePressEvent(QMouseEvent* event);

    //鼠标移动事件
    virtual void mouseMoveEvent(QMouseEvent* event);

	//鼠标释放事件
	virtual void mouseReleaseEvent(QMouseEvent* event);

    void resizeEvent(QResizeEvent* size);

private:
	std::shared_ptr<VideoControl> m_videoControl;

	//底部按钮
	QButtonGroup* m_buttonGroup;

    //按钮界面
    QWidget* m_buttonWidget;

	//视频布局
	QGridLayout* m_videoLayout;

	//总体布局
	QVBoxLayout* m_mainLayout;

    //视频Widget
    vWidget* m_vWidget;

	//是否有视频被鼠标按下
	bool m_videoPressed;

	//是否移动视频
	bool m_videoMove;

	//是否有视频存在
	bool m_videoExist;

	//选中通道Id
	int m_selectChannelId;

	//目标通道Id
	int m_targetChannelId;

	//自定义通道按钮名称
	QString m_name;

	//已添加自定义按钮名称
	std::shared_ptr<std::set<QString>> m_nameSet;

    std::map<QString, QPushButton*> m_cusBtnMap;

	//新增自定义分割
	std::shared_ptr<AddCustomizeChannel> m_addCusChannel;

	//编辑自定义分割
    std::shared_ptr<EditCustomizeChannel> m_editCusChannel;

    //分屏窗口
    ChannelChoose* m_windowingWgt;

    int m_lastSel = -1;

    bool m_isWindowingShow = false;

    bool m_videoingAll = false;

    DeviceTree* m_devTree;

	void InitButton();

	void SetFn();

    //设置图片按钮属性
    void SetPicBtnAttr(QPushButton* btn, const QString& picPath, const QString& hoveredPath, const QString& tipStr);

    //读取自定义数据
    void ReadCus();

public slots:
	void ButtonJudge(int buttonId);

	void CusBtnJudge();

	void AddCusBtn();

    //void EditCusBtn();
};

#endif
