#ifndef QV3VIDEOPLAYINTERFACE_H
#define QV3VIDEOPLAYINTERFACE_H

#include <QWidget>
#include "DataType.h"

class QV3VideoPlayInterface:public QWidget
{
    Q_OBJECT
public:
    QV3VideoPlayInterface();

public:
    //功能：初始化
    void InitDll();

    //功能：登录
    //I/O参数：
    //account:登录账号
    //pwd:登录密码，需进行MD5加密
    //rhost:格式为登录地址:端口，即IP:PORT
    bool Login(const char *account, const char *pwd,
               const char *rhost);

    //功能：获取设备列表信息
    bool GetDeviceList();

    void GetAllMPList(pCMPInfo* &chanlist,int &chancount);

    //功能：请求实时视频
    //I/O参数：puid（设备编号）、chanid（通道编号）、nFactoryCode（厂家代码）、hPlayHwnd（窗口句柄）
    //返 回 值：成功返回0， 失败返回-1
    long StartPlayVideo(const char *puid,const char *chanid,int nFactoryCode,HWND hPlayHwnd);

    //功能：停止单个视频
    //I/O参数：nSessId为StartPlayVideo的返回值
    //返 回 值：成功返回0， 失败返回-1
    long StopPlayVideo(long nSessId);

    //功能：停止所有实时视频
    void StopAllPlayVideo();

    // 功能：云台控制
    // I/O参数：
    //		  nSessId    -- 会话ID
    // 		  szPtzOption        云台控制指令，见xc_video.h中的PTZ_OPTION定义
    // 		  szPtzParam         指令参数，如云台转速等
    bool VideoPtzCtrl(long nSessId, const char* szPtzOption, const char* szPtzParam) ;

    // 功能：开启本地录像
    // I/O参数：
    // nSessId为StartPlayVideo的返回值
    // szFilepath    -- 保存文件路径
    // szFilename  -- 保存文件名称
    // 说明：如果文件已经存在，则覆盖
    bool VideoLocalRecordStart(long nSessId, const char *szFilepath, const char *szFilename);

    // 功能：停止本地录像
    // I/O参数：
    // nSessId为StartPlayVideo的返回值
    bool VideoLocalRecordStop(long nSessId);

    // 功能：播放声音
    // I/O参数：
    // nSessId为StartPlayVideo的返回值
    bool VideoPlaySound(long nSessId);

    // 功能：停止播放声音
    // I/O参数：
    // nSessId为StartPlayVideo的返回值
    bool VideoStopSound(long nSessId);

    //实时视频抓拍
    //I/O参数：
    //nSessId为StartPlayVideo的返回值
    //sFileName-全路径，带后缀名(c:/pic/test.bmp)
    //nPicFmt — 0 - JPEG； 1 - BMP
    bool VideoCapture(long nSessId, const char* sFileName, int nPicFmt);

    //录像回放抓拍
    //I/O参数：
    //sFileName-全路径，带后缀名(c:/pic/test.bmp)
    //nPicFmt — 0 - JPEG； 1 - BMP
    bool RecordCapture(long nSessId, char* sFileName, int nPicFmt);

    //功能：登出
    bool Logout();

    //功能：反初始化
    void UnInitDll();

};

#endif // QV3VIDEOPLAYINTERFACE_H
