#include <queue>
#include "Mysql.h"
#include <mutex>
#include <condition_variable>
using namespace std;

class Connection_Pool
{
private:
    /* data */
    string m_ip;
    string m_user;
    string m_passwd;
    string m_dbName;
    unsigned short m_port;
    int m_minsize;//连接池内的最小连接数
    int m_maxsize;//连接池内的最大连接数
    int m_timeout;
    int m_maxIdleTime;
    queue<Mysql*>My_connectionQ;//使用队列存储连接池
    mutex m_mutexQ;//锁
    condition_variable m_cond;
    bool pareJsonFile();
    void producerConnection();
    void recyclerConnection();
    void addConnection();
public:
    Connection_Pool(/* args */);
    ~Connection_Pool();
    static Connection_Pool* getConnectPool();
    shared_ptr<Mysql> getConect();

};



