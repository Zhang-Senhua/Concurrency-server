#include <iostream>

using namespace std;

#include "System.h"
#include "IniFile.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "Server.h"
using namespace yazi::server;
#include <stdio.h>

int main()
{
    //系统的初始化
    System *sys = Singleton<System>::instance();
    sys->init();
    cout << "                   _ooOoo_" << endl;
    cout << "                  o8888888o" << endl;
    cout << "                  88\" . \"88" << endl;
    cout << "                  (| -_- |)" << endl;
    cout << "                  O\\  =  /O" << endl;
    cout << "               ____/\\`---'\\____" << endl;
    cout << "             .'  \\\\|  %  |//  `." << endl;
    cout << "            /  \\\\|||  %  |||//  \\" << endl;
    cout << "           /  _||||| -%- |||||-  \\" << endl;
    cout << "           |   | \\\\\\  -  /// |   |" << endl;
    cout << "           | \\_|  \'\'\\---/''  |   |" << endl;
    cout << "           \\  .-\\__  `-`  ___/-. /" << endl;
    cout << "         ___`. .'  /--.--\\  `. . __" << endl;
    cout << "      .\"\" \'<  \\`.___\\_<|>_/___.\'  >\'\"\"." << endl;
    cout << "     | | :  \\`- \\\\`.;\\`\\ _ /\\`;.\\`/ - \\` : | |" << endl;
    cout << "     \\  \\ \\`-.   \\_ __\\ /__ _/   .-\\` /  /" << endl;
    cout << "======\\`-.____\\`-.___\\_____/___.-\\`____.-\'======" << endl;
    cout << "                   \\`=---=\'" << endl;
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
    cout << "          佛祖保佑          永不宕机         " << endl;                     /**/
    cout << "      线程池初始化成功，连接池初始化成功，数据库连接成功         " << endl; /**/
    cout << "       最大客户端连接数50000，epoll轮询10ms,线程数1500         " << endl;   /**/
    IniFile *ini = Singleton<IniFile>::instance();
    const string &ip = (*ini)["server"]["ip"];
    int port = (*ini)["server"]["port"];
    int threads = (*ini)["server"]["threads"];
    int max_conn = (*ini)["server"]["max_conn"];
    int wait_time = (*ini)["server"]["wait_time"];

    Server *server = Singleton<Server>::instance();
    server->set_threads(threads);
    server->set_connects(max_conn);
    server->set_wait_time(wait_time);

    server->listen(ip, port);

    server->start();
    printf("服务器已启动\n:ip %s ,端口：%d", ip.c_str(), port);
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