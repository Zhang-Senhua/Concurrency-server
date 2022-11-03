#include <iostream>

using namespace std;

#include "System.h"
#include "IniFile.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "Server.h"
using namespace yazi::server;
#include<stdio.h>

#include"mysql/Mysql.h"

int main()
{
    // 系统的初始化
    System * sys = Singleton<System>::instance();
    sys->init();

    IniFile * ini = Singleton<IniFile>::instance();
    const string & ip = (*ini)["server"]["ip"];
    int port = (*ini)["server"]["port"];
    int threads = (*ini)["server"]["threads"];
    int max_conn = (*ini)["server"]["max_conn"];
    int wait_time = (*ini)["server"]["wait_time"];

    Server * server = Singleton<Server>::instance();
    server->set_threads(threads);
    server->set_connects(max_conn);
    server->set_wait_time(wait_time);
    
    server->listen(ip, port);
    server->start();

    return 0;
}
//对于单例模式的使用
// Do not use a Singleton if:

// If you want to save memory
// If you want to try something new
// If you want to show off how much you know
// Because everyone else is doing it (See cargo cult programmer in wikipedia)
// In user interface widgets
// It is supposed to be a cache
// In strings
// In Sessions
// I can go all day long