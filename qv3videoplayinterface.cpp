#include "qv3videoplayinterface.h"
#include <QElapsedTimer>
#include <QCoreApplication>

#include "LoginMan.h"
#include "DevMan.h"
#include "xc_video.h"
#include "xc_record.h"
#include "qglobaldata.h"
#include "playerbase.h"

QV3VideoPlayInterface::QV3VideoPlayInterface()
{
}


void QV3VideoPlayInterface::InitDll()
{
    REG_Init();
    DEVS_Init();
    VIDEO_Init(1);

    QGlobalData *pGlobalData = QGlobalData::GetInstance();
    QString dllPath = pGlobalData->WorkDirPath;
    dllPath += "/FactoryPlayer";

    PLAYER_Init(dllPath.toStdString().c_str(), NULL);

    pGlobalData->Destroy();
}

bool QV3VideoPlayInterface::Login(const char *account, const char *pwd, const char *rhost)
{
    return REG_Login(account,pwd,rhost);
}

bool QV3VideoPlayInterface::GetDeviceList()
{
    bool bFinished = false;
    if(DEVS_DoGetMPListReq(NULL))
    {
        bFinished = DEVS_IsFinishedMPList();
        while(!bFinished)
        {
            QElapsedTimer time;
            time.start();
            while(time.elapsed() < 100)             //等待时间流逝100毫秒
                QCoreApplication::processEvents();

            bFinished = DEVS_IsFinishedMPList();
            if(bFinished)
                return true;
        }
    }

    return  true;
}

void QV3VideoPlayInterface::GetAllMPList(pCMPInfo* &chanlist,int &chancount)
{
    DEVS_GetAllMPList(&chanlist,&chancount);
}

long QV3VideoPlayInterface::StartPlayVideo(const char *puid,const char *chanid,int nFactoryCode,HWND hPlayHwnd)
{
    if(NULL==puid || NULL==chanid || nFactoryCode<0 || NULL==hPlayHwnd)
        return -1;

    long lSsessId = -1;
    QGlobalData *pGlobalData = QGlobalData::GetInstance();
    QString stOption = pGlobalData->FormVideoPlayOptions();
    lSsessId = VIDEO_Play(puid, chanid, nFactoryCode, hPlayHwnd, stOption.toStdString().c_str(), (void *)hPlayHwnd);
    pGlobalData->Destroy();

    return lSsessId;
}

long QV3VideoPlayInterface::StopPlayVideo(long nSessId)
{
    if(nSessId<0)
        return -1;

    return VIDEO_Stop(nSessId);
}

void QV3VideoPlayInterface::StopAllPlayVideo()
{
    VIDEO_StopAll();
}

bool QV3VideoPlayInterface::VideoPtzCtrl(long nSessId, const char* szPtzOption, const char* szPtzParam)
{
    return VIDEO_PtzCtrl(nSessId,szPtzOption,szPtzParam);
}

bool QV3VideoPlayInterface::VideoLocalRecordStart(long nSessId, const char *szFilepath, const char *szFilename)
{
    return VIDEO_LocalRecordStart(nSessId,szFilepath,szFilename);
}

bool QV3VideoPlayInterface::VideoLocalRecordStop(long nSessId)
{
    return VIDEO_LocalRecordStop(nSessId);
}

bool QV3VideoPlayInterface::VideoPlaySound(long nSessId)
{
    return VIDEO_PlaySound(nSessId);
}

bool QV3VideoPlayInterface::VideoStopSound(long nSessId)
{
    return VIDEO_StopSound(nSessId);
}

bool QV3VideoPlayInterface::VideoCapture(long nSessId, const char* sFileName, int nPicFmt)
{
    return VIDEO_CatchPic(nSessId,sFileName,nPicFmt);
}

bool QV3VideoPlayInterface::RecordCapture(long nSessId, char* sFileName, int nPicFmt)
{
    return (0==RECORD_PlayCatchPic(nSessId,sFileName,nPicFmt))?true:false;
}

bool QV3VideoPlayInterface::Logout()
{
    REG_Logout();
    return  true;
}


void QV3VideoPlayInterface::UnInitDll()
{
    VIDEO_Release();
    REG_Release();
}

