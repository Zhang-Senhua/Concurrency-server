#include "/usr/include/mysql/mysql.h"
#include<iostream>
using namespace std;
class Mysql
{
    /*封装mysql类
    

   
  
    5.查询数据库
    */
private:
    MYSQL* My_conn=nullptr;
    MYSQL_RES* My_result=nullptr;
    MYSQL_ROW My_row=nullptr;
    void freeResult(); //需要对结果集的内存进行手动释放


    /* data */
public:
//1.初始化数据库连接
    Mysql(/* args */);
//2.释放数据库连接
    ~Mysql();
//3.连接数据库
    bool Mysql_conn(string user,string passwd,string dbName,string ip,unsigned short port =3306);
//4.更新数据库
    bool Mysql_update(string sql_cmd);
//5.查询数据库
    bool Mysql_query(string sql_cmd);
//6.遍历查询得到的结果集
    bool Mysql_next();
    string Value(int index);
//7.事务操作
    bool Mysql_transcation();
//8.提交事务
    bool Mysql_commit();
//9.事务回滚
    bool Mysql_rollback();

};



