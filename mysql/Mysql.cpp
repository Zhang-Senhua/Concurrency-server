#include "Mysql.h"
Mysql::Mysql(/* args */)
{
    My_conn=mysql_init(nullptr);
    //设置编码
    mysql_set_character_set(My_conn,"utf8");  
}

Mysql::~Mysql()
{
    if(My_conn!=nullptr)
    {
       mysql_close(My_conn); 
    }
}

 bool Mysql::Mysql_conn(string user,string passwd,string dbName,string ip,unsigned short port =3306)
 {

   MYSQL* ptr= mysql_real_connect(My_conn,ip.c_str(),user.c_str(),passwd.c_str(),dbName.c_str(),port,nullptr,0);
   return ptr !=nullptr;

 }