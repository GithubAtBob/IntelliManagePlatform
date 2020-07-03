#include <stdexcept>
#include <exception>
#include <stdio.h>
#include "ConnectionPool.h"

using namespace std;
using namespace sql;

ConnPool* ConnPool::m_connPool = NULL;

// 获取连接池对象，单例模式
ConnPool* ConnPool::GetInstance()
{
    if (m_connPool == NULL)
    {
        m_connPool = new ConnPool("tcp://127.0.0.1:3306", "root", "1234", 20);
    }

    return m_connPool;
}

// 数据库连接池的构造函数
ConnPool::ConnPool(string url, string userName, string password, int maxSize)
{
    m_maxSize = maxSize;
    m_curSize = 0;
    m_username = userName;
    m_password = password;
    m_url = url;

    try
    {
        m_driver = sql::mysql::get_driver_instance();
    }
    catch (sql::SQLException& e)
    {
        perror("get driver error.\n");
    }
    catch (std::runtime_error& e)
    {
        perror("[ConnPool] run time error.\n");
    }

    // 在初始化连接池时，建立一定数量的数据库连接
    InitConnection(maxSize/2);
}

// 初始化数据库连接池，创建最大连接数一半的连接数量
void ConnPool::InitConnection(int iInitialSize)
{
    Connection* conn;
    lock_guard<mutex> lock(m_lock);
    for (int i = 0; i < iInitialSize; i++)
    {
        conn = CreateConnection();
        if (conn)
        {
            m_connList.push_back(conn);
            ++(m_curSize);
        }
        else
        {
            perror("Init connection error.");
        }
    }
}

// 创建并返回一个连接
Connection* ConnPool::CreateConnection()
{
    Connection* conn;
    try
    {
        // 建立连接
        conn = m_driver->connect(m_url, m_username, m_password);
        return conn;
    }
    catch (sql::SQLException& e)
    {
        perror("create connection error.");
        return NULL;
    }
    catch (std::runtime_error& e)
    {
        perror("[CreateConnection] run time error.");
        return NULL;
    }
}

// 从连接池中获得一个连接
Connection* ConnPool::GetConnection()
{
    Connection* con;
    lock_guard<mutex> lock(m_lock);

    // 连接池容器中还有连接
    if (m_connList.size() > 0)
    {
        // 获取第一个连接
        con = m_connList.front();
        // 移除第一个连接
        m_connList.pop_front();
        // 判断获取到的连接的可用性
        // 如果连接已经被关闭，删除后重新建立一个
        if (con->isClosed())
        {
            delete con;
            con = CreateConnection();
            // 如果连接为空，说明创建连接出错
            if (con == NULL)
            {
                // 从容器中去掉这个空连接
                --m_curSize;
            }
        }
        return con;
    }
    // 连接池容器中没有连接
    else
    {
        // 当前已创建的连接数小于最大连接数，则创建新的连接
        if (m_curSize < m_maxSize)
        {
            con = CreateConnection();
            if (con)
            {
                ++m_curSize;
                return con;
            }
            else
            {
                return NULL;
            }
        }
        // 当前建立的连接数已经达到最大连接数
        else
        {
            perror("[GetConnection] connections reach the max number.");
            return NULL;
        }
    }
}

// 释放数据库连接，将该连接放回到连接池中
void ConnPool::ReleaseConnection(sql::Connection* conn)
{
    if (conn)
    {
        lock_guard<mutex> lock(m_lock);
        m_connList.push_back(conn);
    }
}

// 数据库连接池的析构函数
ConnPool::~ConnPool()
{
    DestoryConnPool();
}

// 销毁连接池，需要先销毁连接池的中连接
void ConnPool::DestoryConnPool()
{
    list<Connection*>::iterator itCon;
    lock_guard<mutex> lock(m_lock);

    for (itCon = m_connList.begin(); itCon != m_connList.end(); ++itCon)
    {
        // 销毁连接池中的连接
        DestoryConnection(*itCon);
    }
    m_curSize = 0;
    // 清空连接池中的连接
    m_connList.clear();
}

// 销毁数据库连接
void ConnPool::DestoryConnection(Connection* conn)
{
    if (conn)
    {
        try
        {
            // 关闭连接
            conn->close();
        }
        catch(sql::SQLException& e)
        {
            perror(e.what());
        }
        catch(std::exception& e)
        {
            perror(e.what());
        }
        // 删除连接
        delete conn;
    }
}
