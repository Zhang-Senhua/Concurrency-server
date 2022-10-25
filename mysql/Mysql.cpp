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
    freeResult();
}

 bool Mysql::Mysql_conn(string user,string passwd,string dbName,string ip,unsigned short port =3306)
 {

   MYSQL* ptr= mysql_real_connect(My_conn,ip.c_str(),user.c_str(),passwd.c_str(),dbName.c_str(),port,nullptr,0);
   return ptr !=nullptr;

 }

  bool Mysql::Mysql_update(string sql_cmd)
  {
    if(mysql_query(My_conn,sql_cmd.c_str()))
    {
        return false;
    }
    //返回非0，执行失败
    return true;

  }

  //5.查询数据库
bool Mysql::Mysql_query(string sql_cmd)
{
    freeResult();
    if(mysql_query(My_conn,sql_cmd.c_str()))
    {
        return false;
    }
    My_result=mysql_store_result(My_conn);
    //返回非0，执行失败
    return true;

}

bool Mysql::Mysql_next()
{
    if(My_result !=nullptr)
    {
         My_row=mysql_fetch_row(My_result);
        return false;

    }
    return true;
}

string Mysql::Value(int index)
{
    char* val=My_row[index];
    int Colum_count=mysql_num_fields(My_result);
    if(index >=Colum_count||index <0)
    {
        return string ();
    }
    unsigned long length=mysql_fetch_lengths(My_result)[index];
    return string(val,length);
}

bool Mysql::Mysql_transcation()
{
    return mysql_autocommit(My_conn,false);
}

bool Mysql::Mysql_commit()
{
    return mysql_commit(My_conn);
}

bool Mysql::Mysql_rollback()
{
    return mysql_rollback(My_conn);
}

void Mysql::freeResult()
{
    if(My_result)
    {
        mysql_free_result(My_result);
        My_result=nullptr;
    }

}