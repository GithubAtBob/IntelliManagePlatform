#include "ToolInstance.h"
#include "sqlite3.h"
#include <QSettings>
#include <QCoreApplication>
#include <cstdlib>
#include <ctime>

ToolInstance* ToolInstance::m_toolInstance = nullptr;

ToolInstance* ToolInstance::GetInstance()
{
    if (m_toolInstance == nullptr)
    {
        m_toolInstance = new ToolInstance;
    }
    return m_toolInstance;
}

QString ToolInstance::ReadIni(QString path, const QString key) const
{
    QSettings* iniRead = new QSettings(path, QSettings::IniFormat);
    QString res = iniRead->value(key).toString();
    delete iniRead;
    return res;
}

void ToolInstance::WriteIni(QString path, const QString key, QString value)
{
    QSettings* iniWrite = new QSettings(path, QSettings::IniFormat);
    iniWrite->setValue(key, value);
    delete iniWrite;
}

sqlite3 *ToolInstance::OpenSqlite(QString name)
{
    static sqlite3* db;
    int rc;
    QString workDirPath = QCoreApplication::applicationDirPath();
    workDirPath += ("/"+name);
    rc = sqlite3_open(workDirPath.toStdString().c_str(), &db);
    if(rc)
    {
       sqlite3_close(db);
       return nullptr;
    }
    else
    {
        return db;
    }
}

std::string ToolInstance::RandFileName(const char *prefix, const char *suffix)
{
    std::string filename,name,time,randNum;
    time = std::to_string(std::time(0));
    randNum = std::to_string(rand());
    name = time + randNum;
    filename = prefix + name + suffix;
    return filename;
}

ToolInstance::ToolInstance()
{

}
