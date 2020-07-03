#ifndef QGLOBALDATA_H
#define QGLOBALDATA_H

#include <QWidget>
#include "DataType.h"

//码流传输模式
enum Stream_Transmission_Mode
{
   STM_UDP,
   STM_TCP
};

class QGlobalData:public QWidget
{
    Q_OBJECT
public:
    QGlobalData();
    static QGlobalData* GetInstance();  //取到QGlobalData对象
    void Destroy();                     //GetInstance()取到的对象释放

private:
    static int _owners;
    static QGlobalData* me;

public:
    int nStreamTransmissionMode;  //码流传输模式
    QString WorkDirPath;

    pCMPInfo* chanlist;  //所有点位列表
    int chancount;       //点位数量

public:
    //播放视频可选参数
    QString FormVideoPlayOptions();
    //获取所有点位列表到本地缓存
    void GetAllMpInfo();
};

#endif // QGLOBALDATA_H
