#include "Connection_Pool.h"
#include <json/json.h>
#include <fstream>
#include <thread>
#include "Logger.h"
using namespace yazi::utility;
using namespace Json;
using namespace yazi::Conpool;

Connection_Pool *Connection_Pool::getConnectPool()
{
    static Connection_Pool Pool;
    return &Pool;
}
bool Connection_Pool::pareJsonFile()
{
    ifstream ifs("mysql/dbconf.json");
    Reader rd;
    Value root;
    rd.parse(ifs, root);
    if (root.isObject())
    {
        m_ip = root["ip"].asString();
        m_user = root["userName"].asString();
        m_passwd = root["password"].asString();
        m_dbName = root["dbName"].asString();
        m_port = root["port"].asInt();
        m_minsize = root["minSize"].asInt();
        m_maxsize = root["maxSize"].asInt();
        m_timeout = root["timeout"].asInt();
        m_maxIdleTime = root["maxIdleTime"].asInt();
        return true;
    }
    else
    {
        return false;
    }
}

Connection_Pool::Connection_Pool(/* args */)
{
}

void Connection_Pool::init()
{
    //加载配置文件
    if (!pareJsonFile())
    {
        error("Connection_Pool pareJsonFile failed!");
        return;
    }
    for (unsigned int i = 0; i < m_minsize; i++)
    {
        Mysql *connect = new Mysql;
        connect->Mysql_conn(m_user, m_passwd, m_dbName, m_ip, m_port);
        My_connectionQ.push(connect);
        //入队列
        debug("create Mysql connect %d", i);
    }
    //生产者消费者模式，一个线程创建维护线程，一个消耗线程
    thread producer(&Connection_Pool::producerConnection, this);
    thread reclycler(&Connection_Pool::recyclerConnection, this);
    producer.detach(); //主线程子线程分离
    reclycler.detach();
}
void Connection_Pool::producerConnection()
{
    //不够用就生产连接，够用就阻塞该线程，节约cpu资源
    while (true)
    {
        unique_lock<mutex> locker(m_mutexQ); // loceker对象自动加解锁，封装后不需要手动进行加锁解锁的操作
        /* code */
        while (My_connectionQ.size() >= m_minsize)
        {
            m_cond.wait(locker); //用wile不用if 避免创建出的连接大于最大连接数
        }
        addConnection();
    }
}
void Connection_Pool::recyclerConnection()
{

    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        unique_lock<mutex> locker(m_mutexQ); // loceker对象自动加解锁，封装后不需要手动进行加锁解锁的操作
        /* code */
        while (My_connectionQ.size() > m_minsize)
        {
            Mysql *con = My_connectionQ.front();
            if (con->getAliveTime() >= -m_maxIdleTime)
            {
                My_connectionQ.pop();
                delete con;
            }
            else
            {
                break;
            }
        }
        addConnection();
        m_cond.notify_all();
    }
}
void Connection_Pool::addConnection()
{
    Mysql *connect = new Mysql;
    connect->Mysql_conn(m_user, m_passwd, m_dbName, m_ip, m_port);
    connect->refreshAlivetime();
    My_connectionQ.push(connect); //入队列
}
shared_ptr<Mysql> Connection_Pool::getConect()
{
    unique_lock<mutex> locker(m_mutexQ); // loceker对象自动加解锁，封装后不需要手动进行加锁解锁的操作
    while (My_connectionQ.empty())
    {
        if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout)))
        {
            if (My_connectionQ.empty())
            {
                continue;
            }
        }
    }
    shared_ptr<Mysql> valuable_conn(My_connectionQ.front(), [this](Mysql *conn)
                                    {
        lock_guard<mutex> locker (m_mutexQ);
        conn->refreshAlivetime();
        My_connectionQ.push(conn);
         m_mutexQ.unlock(); });
    My_connectionQ.pop();
    m_cond.notify_all();
    return valuable_conn;
}

Connection_Pool::~Connection_Pool()
{
    while (!My_connectionQ.empty())
    {
        Mysql *to_delete = My_connectionQ.front();
        My_connectionQ.pop();
        delete to_delete;
    }
}