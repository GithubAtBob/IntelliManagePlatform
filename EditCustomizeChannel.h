#ifndef EDITCUSTOMIZECHANNEL_H
#define EDITCUSTOMIZECHANNEL_H

#include "CusChannelData.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QString>
#include <QGridLayout>
#include <QFrame>
#include <QComboBox>
#include <vector>
#include <memory> 

class EditCustomizeChannel : public QWidget
{
	Q_OBJECT

public:
	EditCustomizeChannel(std::shared_ptr<std::map<QString, CusChannelData>> mapDataPtr, std::shared_ptr<std::set<QString>> nameSet);

	~EditCustomizeChannel();

	//设置在主界面修改/删除自定义通道按钮函数
	void SetCustomizeBtnFn(std::function<void(const QString& name, const QString& rename, bool isDelete, bool isDelCom)> f);

protected:
	//鼠标点击事件
	virtual void mousePressEvent(QMouseEvent* event);

	//鼠标移动事件
	virtual void mouseMoveEvent(QMouseEvent* event);

	//鼠标释放事件
	virtual void mouseReleaseEvent(QMouseEvent* event);

	//绘制事件
	virtual void paintEvent(QPaintEvent* event);

private:
	//重命名名称
	QString m_reCusName;

	//最大通道数
	int m_MaxNum;

	//自定义数据
	std::shared_ptr<std::map<QString, CusChannelData>> m_cusDataMapPtr;

	//已添加自定义名称
	std::shared_ptr<std::set<QString>> m_nameSet;

	//当前名称
	QString m_currName;

	//当前数据
	CusChannelData m_curr;

	//窗口布局
	QGridLayout* m_winLayout;

	//预览窗口
	std::vector<QFrame*> m_previewWinVec;

	//是否已合并
	bool m_hasMerged;

	//鼠标按下起始点
	QPoint m_startpoint;

	//鼠标按下终点
	QPoint m_endpoint;

	//鼠标是否按下
	bool m_mousedown;

	//绘制选中区域
	bool m_paint;

	//下拉选择框
	QComboBox* m_nameCb;

	//在主界面修改/删除自定义通道按钮
	std::function<void(const QString& name, const QString& rename, bool isDelete, bool isDelCom) > m_cusChannelBtnFn;

	//初始化界面
	void InitUi();

	//布局窗口
	void WinLayout();

	//刷新下拉框数据
	void RefreshCombo();

public slots:
	//点击合并按钮
	void MergeClickedSlot();

	//点击取消合并按钮
	void CancelMergeClickedSlot();

	//列数改变
	void ColumChangedSlot(const QString& text);

	//行数改变
	void RowChangedSlot(const QString& text);

	//重命名名称改变
	void RenameChangedSlot(const QString& text);

	//点击修改
	void EditClickedSlot();

	//点击取消
	void CancelClickedSlot();

	//点击删除
	void DeleteClickedSlot();

	//下拉框改变选中项
	void nameChangedSlot(int idx);
};

#endif
