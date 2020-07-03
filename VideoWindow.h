#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include "QVlcPlayer.h"
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QEvent>
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QList>
#include <QAction>
#include <functional>
#include <QKeyEvent>

namespace Video
{
	enum class VideoControlEnum
	{
		Full = 0,
		Four = 1,
		Six = 2,
		Eight = 3,
		Nine = 4,
		Sixteen = 5,
        Twentyfive = 6,
        Customize = 7,
        Num = 8
	};

    const int VideoControlInt[static_cast<int>(VideoControlEnum::Num)] =
	{
            1,
            4,
            6,
            8,
            9,
            16,
            25
	};

}

class QHBoxLayout;
class QTimer;
class QPushButton;
class VideoWindow : public QWidget
{
	Q_OBJECT

public:
    VideoWindow(int i, QWidget *parent = Q_NULLPTR);

	void Play();

	int GetChannelId();

    void SetChannelId(int id);

	void SetUrl(QString url);

	QString GetUrl();

    void Close();

    void CloseAll();

    void Capture();

    void StartVideoing();

    void StopVideoing();

    void SetVideoingBtn(QPushButton* btn);

    void SetBtnState(bool isVideoing);

	void SetChangeChannelFn(std::function<void(Video::VideoControlEnum, int)> f);

	void SetFullFn(std::function<void(int idx)> f);

    void SetCloseAllFn(std::function<void()> f);

    void SetCaptureAllFn(std::function<void()> f);

    void SetVideoingLblFn(std::function<void(bool)> f);

    void SetStartAllVideoingFn(std::function<void()> f);

    void SetStopAllVideoingFn(std::function<void()> f);

	//右键菜单
	virtual void contextMenuEvent(QContextMenuEvent *event);

    //鼠标按下事件
    virtual void mousePressEvent(QMouseEvent* event);

    //鼠标释放事件
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //鼠标移动事件
    virtual void mouseMoveEvent(QMouseEvent* event);

	//鼠标双击事件
	virtual void mouseDoubleClickEvent(QMouseEvent* event);

	//设置选中状态
	void SetSelected();

	//取消选中状态
	void CancelSelected();

    //获取窗口句柄
    HWND Gethwnd();

    //设置是否有视频播放标志
    void SetIsPlay(bool flag);

    //设置是否全屏
    void SetIsFull(bool flag);

    bool IsPlay();

    //设置背景图片
    void SetBackGroundPic();

    //设置背景动图
    void SetBackGroundGif(QString path);

    void StartTimer();

    void ResetCursor();

    void SetIsMove(bool move);

    QTimer *timer;             //定义定时器

    HWND parentwnd;    //准备窗口的句柄

    std::function<void(bool)> m_videoingLblFn;

protected:
    void resizeEvent(QResizeEvent* event);

    //放下动作
    void dropEvent(QDropEvent *event);

    //托到进入窗口动作
    void dragEnterEvent(QDragEnterEvent *event);

    //拖着物体在窗口移动
    void dragMoveEvent(QDragMoveEvent *event);

private:
	//通道Id
	int m_channelId;

	//播放Url
	QString m_url;

    //QLabel* m_channelIdLabel;

//	QWidget* m_flowPanel;

//  QWidget* m_btmflowPanel;

	QMenu* m_menu;

	//右键菜单动作
	QAction* m_switchFullNorAct;

	QAction* m_startPollAct;

	QAction* m_deleteCurrentAct;

	QAction* m_deleteAllAct;

	QAction* m_screenshotCrtAct;

	QAction* m_screenshotAllAct;

    QAction* m_startVideoingCrtAct;

    QAction* m_stopVideoingCrtAct;

    QAction* m_startVideoingAllAct;

    QAction* m_stopVideoingAllAct;

	//切换四通道子菜单
	QMenu* m_switchFourMenu;

	QList<QAction*> m_switchFourActs;

	//切换六通道子菜单
	QMenu* m_switchSixMenu;

	QList<QAction*> m_switchSixActs;

	//切换八通道子菜单
	QMenu* m_switchEightMenu;

	QList<QAction*> m_switchEightActs;

	//切换九通道子菜单
	QMenu* m_switchNineMenu;

	QList<QAction*> m_switchNineActs;

	QAction* m_switchSixteenAct;

	std::function<void(Video::VideoControlEnum, int)> m_changeChannelFn;

    std::function<void(int index)> m_fullFn;

    std::function<void()> m_closeAllFn;

    std::function<void()> m_captureAllFn;

    std::function<void()> m_startAllVideoingFn;

    std::function<void()> m_stopAllVideoingFn;

    //背景图片
    QLabel* m_backImg;

    //QVlcPlayer m_vlcPlayer;

    HWND m_hPlayHwnd;

    //是否有视频在播放
    bool m_isPlay = false;

    bool m_isFull = false;

    QTimer* m_timer;

    QPushButton* m_videoingBtn = nullptr;

    bool m_isMove = false;

	//创建动作
	void CreateActions();

    //设置矩形
    void SetRect(QRect& LeftRect,QRect& TopRect,QRect& RightRect,
                 QRect& BottomRect,QRect& TopLeftRect,QRect& TopRightRect);

    //获取播放返回值
    long GetRet();
public slots:
	//切换全屏/正常模式
	void SwitchFullNorSlot();

	//通道变换
	void ChangeChannel1_4Slot();

	void ChangeChannel5_8Slot();

	void ChangeChannel9_12Slot();

	void ChangeChannel13_16Slot();

	void ChangeChannel1_6Slot();

	void ChangeChannel6_11Slot();

	void ChangeChannel11_16Slot();

	void ChangeChannel1_8Slot();

	void ChangeChannel9_16Slot();

	void ChangeChannel1_9Slot();

	void ChangeChannel8_16Slot();

	void ChangeChannel16();

    void timerprocSlot();

    void JudgeStreamSlot();
};

#endif
