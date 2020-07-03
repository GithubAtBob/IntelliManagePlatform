#include "qglobaldata.h"
#include <QCoreApplication>
#include "qv3videoplayinterface.h"

QGlobalData* QGlobalData::me = NULL;
int QGlobalData::_owners = 0;

QGlobalData::QGlobalData()
{
    nStreamTransmissionMode = STM_TCP;
    WorkDirPath = QCoreApplication::applicationDirPath();

    chanlist = NULL;
    chancount = 0;
}

QGlobalData* QGlobalData::GetInstance()
{
    if(me == NULL)
    {
        me = new QGlobalData;
    }

    _owners++;

    return me;
}

void QGlobalData::Destroy()
{
    _owners--;
    if(_owners<=0)
    {
        if(me)
        {
            delete me;
            me=NULL;
        }
    }
}

QString QGlobalData::FormVideoPlayOptions()
{
    QString stOption = "";
    if (nStreamTransmissionMode == STM_UDP)       //UDP
    {
        stOption += "net_type=0";
    }
    else                                 //TCP
    {
        stOption += "net_type=1";
    }

    stOption += "|";
    stOption += "direct=0";  //分发,不考虑直连了，极少有这个情况了stOption += "direct=1";

    return stOption;
}

void QGlobalData::GetAllMpInfo()
{
    QV3VideoPlayInterface v3VideoPlayInterface;
    v3VideoPlayInterface.GetAllMPList(chanlist,chancount);

//    for(int i=0;i<chancount;i++)
//    {
//            pCMPInfo mpinfo=chanlist[i];
//            if(mpinfo)
//            {;}
//    }
}
