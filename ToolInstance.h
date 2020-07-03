#ifndef TOOLINSTANCE_H
#define TOOLINSTANCE_H

#include <QString>

struct sqlite3;
class ToolInstance
{
public:
    //获取单例
    static ToolInstance* GetInstance();

    //读配置文件
    QString ReadIni(QString path, const QString key) const;

    //写配置文件
    void WriteIni(QString path, const QString key, QString value);

    //打开sqlite 全局变量
    static sqlite3* OpenSqlite(QString name);

    //生成随机文件名
    std::string RandFileName(const char* prefix,const char* suffix);

private:
    ToolInstance();

    static ToolInstance* m_toolInstance;

};

#endif // TOOLINSTANCE_H
