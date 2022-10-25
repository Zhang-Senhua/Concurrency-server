#include "Connection_Pool.h"
#include <json/json.h>
#include<fstream>
using namespace Json;

 Connection_Pool* Connection_Pool::getConnectPool()
 {
    static Connection_Pool Pool;
    return &Pool;

 }
  bool Connection_Pool::pareJsonFile()
  {
    ifstream ifs("dbconf.json");
    Reader rd;
    Value root;
    rd.parse(ifs,root);
    if (root.isObject())
    {
        m_ip=root["ip"].asString();
        m_user=root["userName"].asString();
        m_passwd=root["password"].asString();
        m_dbName=root["dbName"].asString();
        m_port=root["port"].asInt();
        m_minsize=root["minSize"].asInt();
        m_maxsize=root["maxSize"].asInt();
        m_timeout=root["timeout"].asInt();
        m_maxIdleTime=root["maxIdleTime"].asInt();
        return true;

    }
    else{
        return false;
    }
  }
  
