#ifndef ADDCUSTOMIZECHANNEL_H
#define ADDCUSTOMIZECHANNEL_H

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QGridLayout>
#include <utility>
#include <set>
#include <vector>
#include <functional>

class QLabel;
class QPushButton;
class AddCustomizeChannel : public QWidget
{
	Q_OBJECT

public:
    AddCustomizeChannel(QWidget *parent = nullptr);

	//设置在主界面保存自定义通道按钮函数
	void SetCustomizeBtnFn(std::function<void(const QString& name)> f);

	std::pair<int, int> GetCusChannel();

	std::set<int> GetMergeId();

	std::pair<int, int> GetSelRowCol();

protected:
	//鼠标点击事件
	virtual void mousePressEvent(QMouseEvent* event);

	//鼠标移动事件
	virtual void mouseMoveEvent(QMouseEvent* event);

	//鼠标释放事件
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:
	//自定义分割名称
	QString m_cusName;

	//最大通道数
	int m_MaxNum;

	//自定义通道
	std::pair<int, int> m_cusChannel;

	//合并编号
	std::set<int> m_mergeId;

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

    //绘制矩形
    QLabel* m_paintRect;

	//选中通道几行几列
	std::pair<int, int> m_selRowCol;

	//在主界面保存自定义通道按钮
	std::function<void(const QString& name)> m_cusChannelBtnFn;

	//初始化界面
	void InitUi();

public slots:
	//点击合并按钮
	void MergeClickedSlot();

	//点击取消合并按钮
	void CancelMergeClickedSlot();

	//列数改变
    void ColumChangedSlot(const QString& text);

	//行数改变
    void RowChangedSlot(const QString& text);

	//名称改变
	void NameChangedSlot(const QString& text);

	//点击确定
	void SureClickedSlot();

	//点击取消
	void CancelClickedSlot();

    //设置图片按钮属性
    void SetPicBtnAttr(QPushButton* btn, const QString& picPath);
};

#endif
