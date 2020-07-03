#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include "VideoWindow.h"
#include "CusChannelData.h"
#include <QGridLayout>
#include <QString>
#include <map>

class VideoWidget;
class VideoControl
{
public:
    VideoControl();

    //自定义数据写入配置文件
    void WriteCusToIni();

    //自定义数据读取配置文件
    void ReadCusFromIni();

    //自定义数据写入数据库
    void WriteCusToDatabase();

    //自定义数据读取数据库
    void ReadCusFromDatabase();

	//初始化视频信息
    void InitAllWindow(int n, QWidget* prtPtr, bool isReplay = false);

	//显示所有通道画面
    void OpenAll();

	//隐藏所有通道画面
    void CloseAll();

    //截屏所有通道画面
    void CaptureAll();

    //开始录像所有通道画面
    void StartVideoingAll();

    //停止录像所有通道画面
    void StopVideoingAll();

	//显示第i通道的画面
    void Open(int i);

	//隐藏第i通道的画面
    void Close(int i);

	//初始化界面布局
	void InitLayout(int index=0, QString name = "");

	//全屏界面
	void FullVideo(int i);

	//通过id获取窗口下标
	int FindById(int id);

    //通过hwnd获取窗口下标
    int FindByHwnd(HWND hwnd);

	//设置布局样式枚举
	void SetVideoControlEnum(Video::VideoControlEnum e);

	//获取第i个视频窗口
    VideoWindow* GetWindow(int i);

	//获取所有视频窗口
    std::vector<VideoWindow*> GetAllWindow();

	//设置布局
	void SetLayout(QGridLayout* l);

	//设置通道
	void SetChannel(Video::VideoControlEnum e, int index);

	//添加自定义通道数据
    static void AddCusData(const QString& name, CusChannelData data);

	//初始化自定义界面布局
	void InitCusLayout(QString name, int index = 0);

	//获取自定义数据
	std::shared_ptr<std::map<QString, CusChannelData>> GetMapData();

	//获取起始下标
	int GetStartIdx();

	//设置起始下标
	void SetStartIdx(int s);

    //获取窗口句柄
    HWND Gethwnd(int idx);

    void SwapVideoWin(int s, int t);

    void SwapVideoWgt(int s, int t);

    Video::VideoControlEnum GetEnum();

private:
    Video::VideoControlEnum m_videoControlEnum;

	//当前起始下标
	int m_currStartIdx;

	//全局视频窗口
    std::vector<VideoWindow*> m_videoWinVec;

    std::vector<VideoWidget*> m_videoWgtVec;

	//视频布局
	QGridLayout* m_l;

	//最大通道数
	int m_MaxNum;

	//自定义数据
    static std::shared_ptr<std::map<QString, CusChannelData>> m_cusDataMapPtr;

    bool m_first = true;

	//设置窗口可见性
	void SetVisible(int i, int j, bool visible);

	//控件加入布局
    void WgtAddtoLayout(int n, int index=0);

    //sqlite获取数据回调函数
    static int SelectCallback(void *data, int argc, char **argv, char **azColName);
};

#endif
