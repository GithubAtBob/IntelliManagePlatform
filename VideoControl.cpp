#include "VideoControl.h"
#include "DataInstance.h"
#include "ToolInstance.h"
#include "VideoWidget.h"
#include "sqlite3.h"
#include "qv3videoplayinterface.h"
#include <math.h>

std::shared_ptr<std::map<QString, CusChannelData>> VideoControl::m_cusDataMapPtr = std::make_shared<std::map<QString, CusChannelData>>();

VideoControl::VideoControl() : m_videoControlEnum(Video::VideoControlEnum::Four), m_currStartIdx(0), m_videoWinVec(), m_l(nullptr)
{

}

void VideoControl::InitAllWindow(int n, QWidget* prtPtr, bool isReplay)
{
    DataInstance* d = DataInstance::GetInstance();
    d->IniFreePool(n);
	m_videoWinVec.clear();
	m_videoWinVec.reserve(n);
    m_videoWgtVec.clear();
    m_videoWgtVec.reserve(n);
	m_MaxNum = n;
	for (int i = 0; i < n; ++i)
	{
        m_videoWinVec.push_back(new VideoWindow(i, Q_NULLPTR));
		m_videoWinVec[i]->setParent(prtPtr);
		auto changeChannelFn = [this](Video::VideoControlEnum e, int index) {
			SetChannel(e, index);
		};
        m_videoWinVec[i]->SetChangeChannelFn(changeChannelFn);
        auto closeAllFn = [this](){CloseAll();};
        m_videoWinVec[i]->SetCloseAllFn(closeAllFn);
        auto captureAllFn = [this](){CaptureAll();};
        m_videoWinVec[i]->SetCaptureAllFn(captureAllFn);
        auto startAllVideoingFn = [this](){StartVideoingAll();};
        m_videoWinVec[i]->SetStartAllVideoingFn(startAllVideoingFn);
        auto stopAllVideoingFn = [this](){StopVideoingAll();};
        m_videoWinVec[i]->SetStopAllVideoingFn(stopAllVideoingFn);
        m_videoWgtVec.push_back(new VideoWidget(m_videoWinVec[i], prtPtr, isReplay));
	}
}

void VideoControl::OpenAll()
{

}

void VideoControl::CloseAll()
{
    DataInstance* d = DataInstance::GetInstance();
    std::vector<long> ret = d->GetAllRet();
    if(!ret.empty())
    {
        for(auto i : ret)
        {
            QV3VideoPlayInterface v3VideoPlayInterface;
            v3VideoPlayInterface.StopPlayVideo(i);
        }
        for(auto i : m_videoWinVec)
        {
            i->SetIsPlay(false);
            i->setUpdatesEnabled(true);
            i->SetBackGroundPic();
            i->StartTimer();
            i->ResetCursor();
            i->m_videoingLblFn(false);
            i->SetBtnState(false);
            static_cast<VideoWidget*>(i->parentWidget())->SetbPaint(false);
        }
        d->IniFreePool(m_MaxNum);
        d->ClearhwndRetMap();
        d->ClearhwndItemMap();
    }
}

void VideoControl::CaptureAll()
{
    DataInstance* d = DataInstance::GetInstance();
    std::vector<long> ret = d->GetAllRet();
    bool res = false;
    if(!ret.empty())
    {
        for(auto i : ret)
        {
            ToolInstance* t = ToolInstance::GetInstance();
            QV3VideoPlayInterface v3VideoPlayInterface;
            std::string path = t->RandFileName("", ".mp4");
            res = v3VideoPlayInterface.VideoCapture(i, path.c_str(), 0);
        }
    }
}

void VideoControl::StartVideoingAll()
{
    DataInstance* d = DataInstance::GetInstance();
    std::vector<long> ret = d->GetAllRet();
    bool res = false;
    if(!ret.empty())
    {
        for(auto i : ret)
        {
            ToolInstance* t = ToolInstance::GetInstance();
            QV3VideoPlayInterface v3VideoPlayInterface;
            std::string name = t->RandFileName("", ".mp4");
            res = v3VideoPlayInterface.VideoLocalRecordStart(i, "D:\\IntelliManagePlatform\\debug\\video", name.c_str());
        }
        for(auto i : m_videoWinVec)
        {
            if(i->IsPlay())
            {
                i->m_videoingLblFn(true);
                i->SetBtnState(true);
            }
        }
    }
}

void VideoControl::StopVideoingAll()
{
    DataInstance* d = DataInstance::GetInstance();
    std::vector<long> ret = d->GetAllRet();
    bool res = false;
    if(!ret.empty())
    {
        for(auto i : ret)
        {
            ToolInstance* t = ToolInstance::GetInstance();
            QV3VideoPlayInterface v3VideoPlayInterface;
            res = v3VideoPlayInterface.VideoLocalRecordStop(i);
        }
        for(auto i : m_videoWinVec)
        {
            if(i->IsPlay())
            {
                i->m_videoingLblFn(false);
                i->SetBtnState(false);
            }
        }
    }
}

void VideoControl::Open(int i)
{

}

void VideoControl::Close(int i)
{

}

void VideoControl::InitLayout(int index, QString name)
{
    SetVisible(0, m_videoWgtVec.size()-1, false);
	switch (m_videoControlEnum)
     {
		case Video::VideoControlEnum::Full:
		{
			SetVisible(index, index, true);
			WgtAddtoLayout(1, index);
			break;
		}
		case Video::VideoControlEnum::Four:
		{
			SetVisible(index, ((index + 3) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 3), true);
			WgtAddtoLayout(4, index);
			break;
		}
		case Video::VideoControlEnum::Six:
		{
			SetVisible(index, ((index + 5) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 5), true);
			WgtAddtoLayout(6, index);
			break;
		}
		case Video::VideoControlEnum::Eight:
		{
			SetVisible(index, ((index + 7) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 7), true);
			WgtAddtoLayout(8, index);
			break;
        }
		case Video::VideoControlEnum::Nine:
		{
			SetVisible(index, ((index + 8) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 8), true);
			WgtAddtoLayout(9, index);
			break;
		}
		case Video::VideoControlEnum::Sixteen:
		{
			SetVisible(index, ((index + 15) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 15), true);
			WgtAddtoLayout(16, index);
			break;
		}
        case Video::VideoControlEnum::Twentyfive:
        {
            SetVisible(index, ((index + 24) > (m_MaxNum - 1)) ? (m_MaxNum - 1) : (index + 24), true);
            WgtAddtoLayout(25, index);
            break;
        }
		case Video::VideoControlEnum::Customize:
		{
			InitCusLayout(name, index);
			break;
		}
		defaut:
			break;
	}
}

void VideoControl::FullVideo(int i)
{
    m_first = true;
    SetVisible(0, m_videoWgtVec.size()-1, false);
    SetVisible(i, i, true);
    m_l->addWidget(m_videoWgtVec[i], 0, 0, 0, 0);
}

int VideoControl::FindById(int id)
{
	int ret = -1;
    for (std::size_t i=0;i<m_videoWinVec.size();++i)
	{
		if (m_videoWinVec[i]->GetChannelId() == id)
		{
            ret = i;
            break;
		}
	}
    return ret;
}

int VideoControl::FindByHwnd(HWND hwnd)
{
    int ret = -1;
    for (std::size_t i=0;i<m_videoWinVec.size();++i)
    {
        if (m_videoWinVec[i]->Gethwnd() == hwnd)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

void VideoControl::SetVideoControlEnum(Video::VideoControlEnum e)
{
    m_first = true;
    m_videoControlEnum = e;
}

VideoWindow* VideoControl::GetWindow(int i)
{
    return m_videoWinVec[i];
}

std::vector<VideoWindow*> VideoControl::GetAllWindow()
{
	return m_videoWinVec;
}

void VideoControl::SetLayout(QGridLayout * l)
{
	m_l = l;
}

void VideoControl::SetChannel(Video::VideoControlEnum e, int index)
{
	SetVideoControlEnum(e);
	InitLayout(index);
	m_currStartIdx = index;
}

void VideoControl::AddCusData(const QString & name, CusChannelData data)
{
	m_cusDataMapPtr->insert(std::make_pair(name, data));
}

void VideoControl::InitCusLayout(QString name, int index)
{
	auto iter = m_cusDataMapPtr->find(name);
	if (iter == m_cusDataMapPtr->end())
		return;

	CusChannelData d = iter->second;
    SetVisible(0, m_videoWgtVec.size() - 1, false);
	int num = d.m_cusChannel.first*d.m_cusChannel.second - ((d.m_mergeId.size()==0)?1: d.m_mergeId.size());
	SetVisible(index, index + num, true);
	bool first = false;
	int cnt = 0;
	for (int i = 0; i < d.m_cusChannel.second; ++i)
	{
		for (int j = 0; j < d.m_cusChannel.first; ++j)
		{
			int idx = i * d.m_cusChannel.first + j;
			if (d.m_mergeId.find(idx) != d.m_mergeId.end())
			{
				if (!first)
				{
                    first = true;
                    m_l->addWidget(m_videoWgtVec[cnt], i, j, d.m_selRowCol.second, d.m_selRowCol.first);
					++cnt;
				}
				else
				{
					continue;
				}
			}
			else
			{
                m_l->addWidget(m_videoWgtVec[cnt], i, j, 1, 1);
				++cnt;
			}
		}
	}
}

std::shared_ptr<std::map<QString, CusChannelData>> VideoControl::GetMapData()
{
	return m_cusDataMapPtr;
}

int VideoControl::GetStartIdx()
{
	return m_currStartIdx;
}

void VideoControl::SetStartIdx(int s)
{
    m_currStartIdx = s;
}

HWND VideoControl::Gethwnd(int idx)
{
    if(idx >= m_MaxNum || idx < 0)
        return HWND(-1);
    return m_videoWinVec[idx]->Gethwnd();
}

void VideoControl::SwapVideoWin(int s, int t)
{
    std::swap(m_videoWinVec[s], m_videoWinVec[t]);
}

void VideoControl::SwapVideoWgt(int s, int t)
{
    std::swap(m_videoWgtVec[s],m_videoWgtVec[t]);
}

Video::VideoControlEnum VideoControl::GetEnum()
{
    return m_videoControlEnum;
}

void VideoControl::SetVisible(int i, int j, bool visible)
{
	for (int n=i; n<=j; ++n)
	{
        m_videoWgtVec[n]->setVisible(visible);
	}
}

void VideoControl::WgtAddtoLayout(int n, int index)
{
    m_l->setContentsMargins(0, 0, 0, 0);
    m_l->setSpacing(0);
    if (n==1 || n==4 || n==9 || n==16 || n==25)
	{
		int layers = std::sqrt(n);
		for (int i=0; i<layers; ++i)
		{
            for (int j = index; j < index+layers; ++j)
            {
                m_l->addWidget(m_videoWgtVec[i*layers + j], i, j-index, 1, 1);
            }
		}
	}
	else
	{
		int i = index+1;
		int max = n / 2 - 1;
        int x = 0, y = max;
        m_l->addWidget(m_videoWgtVec[index], 0, 0, max, max);
		while (i < index+n)
        {
            m_l->addWidget(m_videoWgtVec[i], x, y, 1, 1);
			if (x < max)
				++x;
			else
				--y;
			++i;
		}
    }
}

int VideoControl::SelectCallback(void *data, int argc, char **argv, char **azColName)
{
    std::string strName = "";
    std::string strData = "";
    for(int i=0; i<argc; i++)
    {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
       if(strcmp(azColName[i],"name")==0)
           strName = argv[i];
       else if(strcmp(azColName[i],"data")==0)
           strData = argv[i];
    }
    QStringList dataList = QString::fromStdString(strData).split(",");
    dataList.removeLast();
    CusChannelData d;
    d.m_selRowCol = std::make_pair(dataList.at(0).toInt(), dataList.at(1).toInt());
    d.m_cusChannel = std::make_pair(dataList.at(2).toInt(), dataList.at(3).toInt());
    for(int j=4;j<dataList.size();++j)
    {
        d.m_mergeId.insert(dataList.at(j).toInt());
    }
    AddCusData(QString::fromStdString(strName), d);
    return 0;
}

void VideoControl::WriteCusToIni()
{
    size_t size = m_cusDataMapPtr->size();
    if(!size)
        return;
    ToolInstance* instance = ToolInstance::GetInstance();
    QString name = "";
    for(auto i = m_cusDataMapPtr->begin();i != m_cusDataMapPtr->end();++i)
    {
        name += i->first;
        name += ",";
        QString data = "";
        auto cusData = i->second;
        data += (QString::number(cusData.m_selRowCol.first) + "," + QString::number(cusData.m_selRowCol.second) + ",");
        data += (QString::number(cusData.m_cusChannel.first) + "," + QString::number(cusData.m_cusChannel.second) + ",");
        for(auto j=cusData.m_mergeId.begin();j!=cusData.m_mergeId.end();++j)
        {
            data += QString::number(*j);
            data += ",";
        }
        instance->WriteIni("D:\\IntelliManagePlatform\\config.ini","/customdata/"+i->first,data);
    }
    instance->WriteIni("D:\\IntelliManagePlatform\\config.ini", "/customdata/name", name);
}

void VideoControl::ReadCusFromIni()
{
    ToolInstance* instance = ToolInstance::GetInstance();
    QString name = instance->ReadIni("D:\\IntelliManagePlatform\\config.ini", "/customdata/name");
    QStringList nameList = name.split(",");
    nameList.removeLast();
    QString data = "";
    for(auto i = nameList.begin();i != nameList.end();++i)
    {
        data = instance->ReadIni("D:\\IntelliManagePlatform\\config.ini", "/customdata/"+(*i));
        QStringList dataList = data.split(",");
        dataList.removeLast();
        CusChannelData d;
        d.m_selRowCol = std::make_pair(dataList.at(0).toInt(), dataList.at(1).toInt());
        d.m_cusChannel = std::make_pair(dataList.at(2).toInt(), dataList.at(3).toInt());
        for(int j=4;j<dataList.size();++j)
        {
            d.m_mergeId.insert(dataList.at(j).toInt());
        }
        AddCusData(*i, d);
    }
}

void VideoControl::WriteCusToDatabase()
{
    /****MYSQL****
    size_t size = m_cusDataMapPtr->size();
    if(!size)
        return;

    ConnPool *connpool = ConnPool::GetInstance();
    Connection *con;
    Statement *state;

    // 从连接池中获取连接
    con = connpool->GetConnection();

    state = con->createStatement();
    state->execute("use IntelliManagePlatform");

    try
    {
        state->executeQuery("truncate table customData");
    }
    catch (sql::SQLException& e)
    {
        perror("get driver error.\n");
    }
    catch (std::runtime_error& e)
    {
        perror("[ConnPool] run time error.\n");
    }

    for(auto i = m_cusDataMapPtr->begin();i != m_cusDataMapPtr->end();++i)
    {
        QString data = "";
        auto cusData = i->second;
        data += (QString::number(cusData.m_selRowCol.first) + "," + QString::number(cusData.m_selRowCol.second) + ",");
        data += (QString::number(cusData.m_cusChannel.first) + "," + QString::number(cusData.m_cusChannel.second) + ",");
        for(auto j=cusData.m_mergeId.begin();j!=cusData.m_mergeId.end();++j)
        {
            data += QString::number(*j);
            data += ",";
        }
        const std::string sqlStr = "insert into customData (name, data) values ('"+(i->first).toStdString()+"','"+data.toStdString()+"')";
        try
        {
            state->executeQuery(sqlStr);
        }
        catch (sql::SQLException& e)
        {
            perror("get driver error.\n");
        }
        catch (std::runtime_error& e)
        {
            perror("[ConnPool] run time error.\n");
        }
    }

    delete state;
    connpool->ReleaseConnection(con);
    *************/

    size_t size = m_cusDataMapPtr->size();
    if(!size)
        return;

    ToolInstance* instance = ToolInstance::GetInstance();
    sqlite3* db = nullptr;
    char* errmsg = 0;
    int rc = 0;
    db = instance->OpenSqlite("intellimanageplatform.db");

    if(db == nullptr)
    {
       return;
    }
    else
    {
        rc = sqlite3_exec(db,"DELETE FROM customData;",NULL,NULL,&errmsg);
        if(rc != SQLITE_OK)
        {
            sqlite3_free(errmsg);
        }
        for(auto i = m_cusDataMapPtr->begin();i != m_cusDataMapPtr->end();++i)
        {
            QString data = "";
            auto cusData = i->second;
            data += (QString::number(cusData.m_selRowCol.first) + "," + QString::number(cusData.m_selRowCol.second) + ",");
            data += (QString::number(cusData.m_cusChannel.first) + "," + QString::number(cusData.m_cusChannel.second) + ",");
            for(auto j=cusData.m_mergeId.begin();j!=cusData.m_mergeId.end();++j)
            {
                data += QString::number(*j);
                data += ",";
            }
            const std::string sqlStr = "insert into customData (name, data) values ('"+(i->first).toStdString()+"','"+data.toStdString()+"');";
            rc = sqlite3_exec(db, sqlStr.c_str(),NULL,NULL,&errmsg);
            if(rc != SQLITE_OK)
            {
                sqlite3_free(errmsg);
            }
        }
    }
    sqlite3_close(db);
}

void VideoControl::ReadCusFromDatabase()
{
    /****MYSQL****
    ConnPool *connpool = ConnPool::GetInstance();
    Connection *con;
    Statement *state;
    ResultSet *result;

    // 从连接池中获取连接
    con = connpool->GetConnection();

    state = con->createStatement();
    state->execute("use IntelliManagePlatform");

    // 查询数据库
    try
    {
        result = state->executeQuery("select name,data from customData");
        while (result->next())
        {
            std::string strName = result->getString("name");
            std::string strData = result->getString("data");
            QStringList dataList = QString::fromStdString(strData).split(",");
            dataList.removeLast();
            CusChannelData d;
            d.m_selRowCol = std::make_pair(dataList.at(0).toInt(), dataList.at(1).toInt());
            d.m_cusChannel = std::make_pair(dataList.at(2).toInt(), dataList.at(3).toInt());
            for(int j=4;j<dataList.size();++j)
            {
                d.m_mergeId.insert(dataList.at(j).toInt());
            }
            AddCusData(QString::fromStdString(strName), d);
        }
    }
    catch (sql::SQLException& e)
    {
        perror("get driver error.\n");
        delete result;
        delete state;
        connpool->ReleaseConnection(con);
    }
    catch (std::runtime_error& e)
    {
        perror("[ConnPool] run time error.\n");
        delete result;
        delete state;
        connpool->ReleaseConnection(con);
    }

    delete result;
    delete state;
    connpool->ReleaseConnection(con);
    *************/

    ToolInstance* instance = ToolInstance::GetInstance();
    sqlite3* db = nullptr;
    char* errmsg = 0;
    int rc = 0;
    db = instance->OpenSqlite("intellimanageplatform.db");

    if(db == nullptr)
    {
       return;
    }
    else
    {
        std::string sql = "SELECT name,data from customData";
        rc = sqlite3_exec(db, sql.c_str(), SelectCallback, NULL,&errmsg);
        if( rc != SQLITE_OK )
        {
            sqlite3_free(errmsg);
        }
    }
    sqlite3_close(db);
}
