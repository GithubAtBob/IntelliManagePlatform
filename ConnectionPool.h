#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <mutex>
#include <list>
#include <string>

using namespace std;
using namespace sql;

class ConnPool
{
public:
    ~ConnPool();
    // 获取数据库连接
    Connection* GetConnection();
    // 将数据库连接放回到连接池的容器中
    void ReleaseConnection(Connection *conn);
    // 获取数据库连接池对象
    static ConnPool* GetInstance();

private:
    // 当前已建立的数据库连接数量
    int m_curSize;
    // 连接池定义的最大数据库连接数
    int m_maxSize;
    string m_username;
    string m_password;
    string m_url;
    // 连接池容器
    list<Connection*> m_connList;
    // 线程锁
    mutex m_lock;
    static ConnPool* m_connPool;
    Driver* m_driver;

    // 创建一个连接
    Connection* CreateConnection();
    // 初始化数据库连接池
    void InitConnection(int iInitialSize);
    // 销毁数据库连接对象
    void DestoryConnection(Connection *conn);
    // 销毁数据库连接池
    void DestoryConnPool();
    // 构造方法
    ConnPool(string url, string user,string password, int maxSize);
};

#endif // CONNECTIONPOOL_H
