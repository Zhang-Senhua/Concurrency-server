#pragma once
#include <queue>
#include "Mysql.h"
#include <mutex>
#include <condition_variable>
using namespace std;
using namespace yazi::MYSql;

namespace yazi
{
    namespace Conpool
    {

        class Connection_Pool
        {
        private:
            /* data */
            string m_ip;
            string m_user;
            string m_passwd;
            string m_dbName;
            unsigned short m_port;
            unsigned int m_minsize; //连接池内的最小连接数
            unsigned int m_maxsize; //连接池内的最大连接数
            int m_timeout;
            int m_maxIdleTime;
            queue<Mysql *> My_connectionQ; //使用队列存储连接池
            mutex m_mutexQ;                //锁
            condition_variable m_cond;
            bool pareJsonFile();
            void producerConnection();
            void recyclerConnection();
            void addConnection();

        public:
            Connection_Pool(/* args */);
            ~Connection_Pool();
            void init();
            static Connection_Pool *getConnectPool();
            shared_ptr<Mysql> getConect();
        };
    }
}
